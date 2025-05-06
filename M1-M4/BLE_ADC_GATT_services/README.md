
# ADC Voltage Monitoring and BLE Notification Example

This example demonstrates how to use the internal ADC of the Silicon Labs device to measure the system voltage (AVDD) and notify the value over BLE. The ADC measurement is shared as both a raw value and a battery percentage through custom GATT characteristics.

## How to Use This Example with Si Connect Mobile Application

1. **Scan for BLE Devices** using the **Si Connect** mobile application.
2. You will find a device advertising with the local name **"ADC Monitor"**.
3. Connect to the device and navigate to the **GATT characteristics**:
   - **Raw ADC Value** characteristic (UUID: Custom) – Notifies the 12-bit ADC raw value (2 bytes).
   - **Battery Level** characteristic (UUID: Standard 0x2A19) – Notifies the battery percentage (0–100%).
4. The ADC is sampled periodically (every 5 seconds), and the data is automatically notified if notifications are enabled.

## BLE Characteristics

| Characteristic Name | UUID        | Type      | Properties       | Description                          |
|---------------------|-------------|-----------|------------------|--------------------------------------|
| ADC Raw Value       | Custom UUID | uint16[2] | Notify, Read     | 12-bit ADC result (e.g., 0–4095)     |
| Battery Level       | 0x2A19      | uint8     | Notify, Read     | Calculated battery percentage (0–100)|

## ADC Configuration

- **Input Source**: AVDD (device supply voltage)
- **Reference Voltage**: 1.2V internal
- **Gain**: 4x
- **Voltage Calculation**:  
  `Voltage (mV) = (ADC_raw * 1250 * 4) / 4095`
- **Battery Range Mapping**:  
  - Min Voltage = 1800 mV (0%)  
  - Max Voltage = 3300 mV (100%)

## GPIO / Hardware Mapping

| Function     | GPIO Pin | Notes              |
|--------------|----------|--------------------|
| ADC Input    | AVDD     | Internal connection|
| Status LED   | PB02     | Optional – for debug indication |

> **Note**: You can optionally configure a status LED to blink or indicate BLE connection state.

## Supported Modules

1. B1BMBG22M1
2. B1BMBG22M4


