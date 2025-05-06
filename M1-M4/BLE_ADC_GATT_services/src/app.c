#include "em_iadc.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "sl_bt_api.h"
#include "gatt_db.h"
#include <stdint.h>
#include <stdio.h>

#define ADC_INPUT_PORT         gpioPortA
#define ADC_INPUT_PIN          3
//#define IADC_INPUT             iadcPosInputPortCPin4
#define ADC_VREF_MV            1200
#define ADC_MAX_COUNT          4095
#define ADC_UPDATE_PERIOD_MS   2000

// BLE handles
uint8_t advertising_set_handle = 0xFF;
uint8_t connection_handle = 0xFF;

// ADC value
uint16_t adc_raw_value = 0;
uint16_t  gatt_db_adc_value;
uint16_t  gattdb_adc_raw_value;

// Upper and lower bound for Window Comparator
static uint16_t vdd_lower_bound = 0x6550;   // 1.9V (raw)
static uint16_t vdd_upper_bound = 0xC540;   // 3.7V (raw)

// voltage rail voltage range in mV
static volatile uint16_t vdd_upper_voltage;
static volatile uint16_t vdd_lower_voltage;

// measured voltage using ADC
static volatile uint16_t voltagemV;

// Read and notify ADC value
void read_adc_value(void)
{
    IADC_Result_t adc_data;

    // Start ADC conversion
    IADC_command(IADC0, iadcCmdStartSingle);
    for (volatile int i = 0; i < 30000; i++); // short delay
    adc_data = IADC_pullSingleFifoResult(IADC0);
    IADC_command(IADC0, iadcCmdStopSingle);

    adc_raw_value = adc_data.data;
    printf(">>> Raw ADC = %u\n", adc_raw_value);

    // Send raw ADC value as 2 bytes
    uint8_t raw_data[2];
    raw_data[0] = (uint8_t)(adc_raw_value & 0xFF);
    raw_data[1] = (uint8_t)((adc_raw_value >> 8) & 0xFF);

    //  Write ADC value to the GATT attribute (adc_value characteristic)
    sl_status_t sc = sl_bt_gatt_server_write_attribute_value(gatt_db_adc_value, 0, 2, raw_data);

    //  Notify all connected BLE clients with the ADC value
    sl_status_t sc_1 = sl_bt_gatt_server_notify_all(gattdb_adc_value, 2, raw_data);

    if (sc_1!= SL_STATUS_OK) {
        printf("Notify RAW ADC failed: 0x%04X\n", sc);
    } else {
        printf("Raw ADC notified: %u\n", adc_raw_value);
    }

    // Voltage calculation (1.2V internal ref, 4x gain)
    voltagemV = (adc_raw_value * 120 * 4) / 4095;
    printf("Voltage : %lu mV\n", voltagemV);

    // Battery %
    uint8_t battery_percent;
    uint16_t min_voltage = 1800;
    uint16_t max_voltage = 3300;

    if (voltagemV <= min_voltage){
        battery_percent = 0;
      }
    else if (voltagemV >= max_voltage) {
        battery_percent = 100;
    }
    else {
        battery_percent = ((voltagemV - min_voltage) * 100) / (max_voltage - min_voltage);
    }

    printf(">>> Battery Level = %u%%\n", battery_percent);

    uint8_t battery_data[1] = { battery_percent };

    // Custom battery % GATT
    sl_bt_gatt_server_write_attribute_value(gatt_db_adc_value, 0, 1, battery_data);
    sl_bt_gatt_server_notify_all(gatt_db_adc_value, 1, battery_data);

    // Standard battery level
    sl_bt_gatt_server_write_attribute_value(gattdb_battery_level, 0, 1, battery_data);
    sl_bt_gatt_server_notify_all(gattdb_battery_level, 1, battery_data);
}


// Initialize ADC with window comparator
void init_adc_with_window_comparator(void)
{
    vdd_upper_voltage = (vdd_upper_bound >> 4) * 1200 * 4 / 4095;
    vdd_lower_voltage = (vdd_lower_bound >> 4) * 1200 * 4 / 4095;

    printf("Initializing ADC with Window Comparator...\n");

    CMU_ClockEnable(cmuClock_IADC0, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    GPIO_PinModeSet(ADC_INPUT_PORT, ADC_INPUT_PIN, gpioModeDisabled, 0);

    IADC_Init_t init = IADC_INIT_DEFAULT;
    IADC_AllConfigs_t allConfigs = IADC_ALLCONFIGS_DEFAULT;
    IADC_InitSingle_t initSingle = IADC_INITSINGLE_DEFAULT;
    IADC_SingleInput_t singleInput = IADC_SINGLEINPUT_DEFAULT;

    allConfigs.configs[0].reference = iadcCfgReferenceInt1V2;
    allConfigs.configs[0].adcMode = iadcCfgModeNormal;
    allConfigs.configs[0].osrHighSpeed = iadcCfgOsrHighSpeed2x;

//    init.warmup = iadcWarmupKeepWarm;
    init.srcClkPrescale = IADC_calcSrcClkPrescale(IADC0, 10000000, 0);

    init.greaterThanEqualThres = vdd_upper_bound;
    init.lessThanEqualThres = vdd_lower_bound;

    initSingle.triggerAction = iadcTriggerActionContinuous;
    singleInput.posInput = iadcPosInputAvdd;
    singleInput.negInput = iadcNegInputGnd;
    singleInput.compare = true;

    IADC_init(IADC0, &init, &allConfigs);
    IADC_initSingle(IADC0, &initSingle, &singleInput);

    IADC_enableInt(IADC0, IADC_IF_SINGLECMP);
    NVIC_ClearPendingIRQ(IADC_IRQn);
    NVIC_EnableIRQ(IADC_IRQn);

    IADC_command(IADC0, iadcCmdStartSingle);

    printf("ADC with Window Comparator initialized.\n");
}

// ADC IRQ Handler
void IADC_IRQHandler(void)
{
    IADC_clearInt(IADC0, IADC_IF_SINGLECMP);
    IADC_Result_t adc_raw_value = IADC_readSingleResult(IADC0);
    uint32_t voltagemV = (adc_raw_value.data) * 4 * 1200 / 4095;

    printf("Voltage : %dmV\n", voltagemV);

    IADC_command(IADC0, iadcCmdStartSingle);
}

// BLE Event Handler
void sl_bt_on_event(sl_bt_msg_t *evt)
{
    sl_status_t sc;

    switch (SL_BT_MSG_ID(evt->header)) {
        case sl_bt_evt_system_boot_id:
            printf("Bluetooth System Booted. Initializing Advertising...\n");

            sc = sl_bt_advertiser_create_set(&advertising_set_handle);
            sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle, sl_bt_advertiser_general_discoverable);
            sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_legacy_advertiser_connectable);

            if (sc == SL_STATUS_OK) {
                printf("Advertising started!\n");
            } else {
                printf("Failed to start advertising: 0x%04X\n", sc);
            }
            break;

        case sl_bt_evt_connection_opened_id:
            printf("BLE Connected. Sending initial ADC value...\n");

            // Notify latest ADC value on connection
            sl_bt_gatt_server_notify_all(
                gattdb_adc_value,
                sizeof(adc_raw_value),
                (uint8_t *)&adc_raw_value
            );
            break;

        case sl_bt_evt_connection_closed_id:
            printf("Disconnected.\n");
            connection_handle = 0xFF;

            // Restart advertising
            sc = sl_bt_advertiser_start(advertising_set_handle,
                                        sl_bt_advertiser_general_discoverable,
                                        sl_bt_advertiser_connectable_scannable);
            if (sc == SL_STATUS_OK) {
                printf("Re-advertising started.\n");
            } else {
                printf("Failed to restart advertising. Error: 0x%04X\n", sc);
            }
            break;

        case sl_bt_evt_system_soft_timer_id:
            if (evt->data.evt_system_soft_timer.handle == 0) {
                read_adc_value();
            }
            break;

        case sl_bt_evt_gatt_server_characteristic_status_id:
            if (evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_client_config) {
                uint16_t flags = evt->data.evt_gatt_server_characteristic_status.client_config_flags;
                printf("Notifications %s\n", (flags & sl_bt_gatt_server_notification) ? "ENABLED" : "DISABLED");
            }
            break;

        default:
            break;
    }
}

// App Init
void app_init(void)
{
    printf("App Init...\n");

    init_adc_with_window_comparator();

    sl_status_t sc = sl_bt_system_set_lazy_soft_timer(
                        32768 * 5,      // 5 seconds
                        32768 / 5,      // 200 ms slack
                        0,              // Timer handle
                        0               // Periodic
                    );

    if (sc != SL_STATUS_OK) {
        printf("Failed to start ADC timer. Error: 0x%04X\n", sc);
    }
}

// App Main Loop
void app_process_action(void)
{
    // Nothing to do in main loop
}
