#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_system.h"
#include "sl_power_manager.h"
#include "sl_sleeptimer.h"
#include "app_log.h"
#include "app_assert.h"
#include "nvm3.h"
#include "nvm3_default.h"
#include "sl_bt_api.h"
#include "gatt_db.h"  // Include generated GATT DB definitions

#include <stdbool.h>
#include <stdint.h>

// Define GPIO pins for buttons
#define BUTTON_1_PORT gpioPortB
#define BUTTON_1_PIN  1
#define BUTTON_2_PORT gpioPortC
#define BUTTON_2_PIN  1

// NVM3 key
#define RESTART_COUNT_KEY 0x4000

static uint32_t restart_count = 0;
static bool already_pressed = false;
static uint8_t connection_handle = 0xFF;
static uint8_t advertising_set_handle = 0xFF;
static bool notifications_enabled = false;

// Read restart count from NVM
static uint32_t read_restart_count(void)
{
    uint32_t count = 0;
    Ecode_t err = nvm3_readData(nvm3_defaultHandle, RESTART_COUNT_KEY, &count, sizeof(count));
    if (err != ECODE_NVM3_OK) {
        app_log("[WARN] NVM3 read failed, defaulting to 0\n");
        count = 0;
    }
    return count;
}

// Write restart count to NVM
static void write_restart_count(uint32_t count)
{
    Ecode_t err = nvm3_writeData(nvm3_defaultHandle, RESTART_COUNT_KEY, &count, sizeof(count));
    app_assert(err == ECODE_NVM3_OK, "[ERROR] NVM3 write failed!\n");
}

// Notify BLE client of restart count
void update_restart_count_ble(uint32_t count)
{
    app_log("[DEBUG] Notifications enabled = %d, connection_handle = 0x%02X\n",
             notifications_enabled, connection_handle);

    if (!notifications_enabled || connection_handle == 0xFF) {
        app_log("[WARN] Cannot notify: Notifications not enabled or no connection\n");
        return;
    }

    uint8_t value[4];
    value[0] = count & 0xFF;
    value[1] = (count >> 8) & 0xFF;
    value[2] = (count >> 16) & 0xFF;
    value[3] = (count >> 24) & 0xFF;

    sl_status_t sc;

    // Write value to GATT attribute
    sc = sl_bt_gatt_server_write_attribute_value(gattdb_restart_count, 0, sizeof(value), value);
    if (sc != SL_STATUS_OK) {
        app_log("[ERROR] Write to GATT attribute failed: 0x%04X\n", sc);
    }

    // Send notification to connected client
    sc = sl_bt_gatt_server_send_notification(connection_handle, gattdb_restart_count, sizeof(value), value);
    if (sc != SL_STATUS_OK) {
        app_log("[ERROR] Notification failed: 0x%04X\n", sc);
    } else {
        app_log("[INFO] Restart count notified: %lu\n", count);
    }
}

// Reset restart count
static void reset_restart_count(void)
{
    restart_count = 0;
    write_restart_count(restart_count);
    update_restart_count_ble(restart_count);
    app_log("[INFO] Restart count reset to 0\n");
}

// Increment restart count
static void increment_restart_count(void)
{
    restart_count = read_restart_count();
    restart_count++;
    write_restart_count(restart_count);
    app_log("[INFO] Restart count incremented: %lu\n", restart_count);
}

// GPIO button init
static void init_buttons(void)
{
    CMU_ClockEnable(cmuClock_GPIO, true);
    GPIO_PinModeSet(BUTTON_1_PORT, BUTTON_1_PIN, gpioModeInputPull, 1);
    GPIO_PinModeSet(BUTTON_2_PORT, BUTTON_2_PIN, gpioModeInputPull, 1);
    app_log("[INFO] Buttons initialized on PB%d and PC%d\n", BUTTON_1_PIN, BUTTON_2_PIN);
}

// Check both button state
static bool are_buttons_pressed(void)
{
    return (GPIO_PinInGet(BUTTON_1_PORT, BUTTON_1_PIN) == 0 &&
            GPIO_PinInGet(BUTTON_2_PORT, BUTTON_2_PIN) == 0);
}

// BLE event handler
void sl_bt_on_event(sl_bt_msg_t *evt)
{
    sl_status_t sc;

    switch (SL_BT_MSG_ID(evt->header)) {

        case sl_bt_evt_system_boot_id:
            app_log("[INFO] Bluetooth Booted. Starting Advertising...\n");

            sc = sl_bt_advertiser_create_set(&advertising_set_handle);
            app_assert_status(sc);

            sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                       sl_bt_advertiser_general_discoverable);
            app_assert_status(sc);

            sc = sl_bt_advertiser_set_timing(advertising_set_handle, 160, 160, 0, 0);
            app_assert_status(sc);

            sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                               sl_bt_legacy_advertiser_connectable);
            app_assert_status(sc);

            app_log("[INFO] BLE advertising started\n");
            break;

        case sl_bt_evt_connection_opened_id:
            connection_handle = evt->data.evt_connection_opened.connection;
            notifications_enabled = false;
            app_log("[INFO] BLE connected. Handle: %d\n", connection_handle);
            break;

        case sl_bt_evt_gatt_server_characteristic_status_id:
            if (evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_client_config) {
                uint16_t flags = evt->data.evt_gatt_server_characteristic_status.client_config_flags;
                notifications_enabled = (flags & sl_bt_gatt_notification);
                app_log("[INFO] Notifications %s by client\n",
                        notifications_enabled ? "ENABLED" : "DISABLED");

                if (notifications_enabled) {
                    update_restart_count_ble(restart_count);
                }
            }
            break;

        case sl_bt_evt_connection_closed_id:
            app_log("[INFO] BLE disconnected\n");
            connection_handle = 0xFF;
            notifications_enabled = false;

            sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle,
                                                       sl_bt_advertiser_general_discoverable);
            app_assert_status(sc);

            sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                               sl_bt_legacy_advertiser_connectable);
            app_assert_status(sc);

            app_log("[INFO] Re-advertising started\n");
            break;

        case sl_bt_evt_system_soft_timer_id:
            if (evt->data.evt_system_soft_timer.handle == 1) {
                update_restart_count_ble(restart_count);
            }
            break;

        default:
            break;
    }
}

// App init
void app_init(void)
{
    CHIP_Init();
    init_buttons();
    increment_restart_count();
    app_log("[INFO] Application initialized\n");
}

// App loop
void app_process_action(void)
{
    if (are_buttons_pressed())
    {
        if (!already_pressed)
        {
            app_log("[INFO] Both buttons pressed. Resetting restart count...\n");
            reset_restart_count();
            already_pressed = true;
            sl_sleeptimer_delay_millisecond(300); // Debounce
        }
    }
    else
    {
        already_pressed = false;
    }
}
