# Restart Count Tracker with BLE Notifications

This project implements a **BLE-based restart counter** using **Silicon Labs' embedded platform**. It tracks device restarts via non-volatile memory (NVM3), displays this information through debug logs, and allows BLE clients to subscribe to receive the count through GATT notifications. The restart count can also be reset using a simultaneous button press.

---

## Features

-  **Persistent Restart Count** using **NVM3**
-  **Button-Triggered Reset** with debounce handling
-  **BLE Advertising & Connection Support**
-  **BLE Notification Support** for Restart Count
-  **Logging** with App Log System

---

## Hardware Requirements

- Silicon Labs development board (e.g., **B1BMBG22M1**, **B1BMBG22M4**, etc.)
- Two push buttons connected to:
  - **PB1**
  - **PC1**
- Optional: BLE mobile app (e.g., *nRF Connect*, *Silicon Labs Connect*) for testing GATT interactions

---

## GPIO Configuration

| Button | Port | Pin |
|--------|------|-----|
| BTN1   | B    | 1   |
| BTN2   | C    | 1   |

---

## How It Works

1. On each boot, the device:
   - Reads the restart count from NVM3.
   - Increments and stores it back to NVM3.
   - Logs the new count via `app_log`.

2. On **BLE connection**, the client can subscribe to notifications for the restart count characteristic.

3. On **both buttons being pressed simultaneously**, the restart count is reset to 0, updated in NVM3, and notified to the connected client (if notifications are enabled).

---

## GATT Details

A custom GATT characteristic (`gattdb_restart_count`) is used to:
- Notify the BLE client of the restart count.
- Support notifications once the client enables them via the Client Characteristic Configuration Descriptor (CCCD).

---

## Build & Flash Instructions

1. Open the project in **Simplicity Studio v5**.
2. Make sure your **GATT configuration** includes a `restart_count` characteristic with **notify** property.
3. Connect your development board.
4. Build and flash the firmware to the device.
5. Open a **serial terminal** (e.g., Simplicity Terminal) to view debug logs.

---

## BLE Testing Steps

1. Use a BLE mobile app (e.g., *nRF Connect*):
   - Scan for the device.
   - Connect to the device.
   - Subscribe to the `restart_count` characteristic.
   - Observe the initial count notification.

2. Disconnect and reset the board.
   - Reconnect and verify that the count is incremented.

3. Press **both buttons simultaneously** to reset the count.
   - A notification with value `0` will be sent if subscribed.

---

## Important Notes

- The code uses **Soft Timer ID 1** to optionally notify the client periodically (depending on use case).
- `sl_bt_on_event()` handles BLE stack events for connection, disconnection, and notifications.
- Ensure `gattdb_restart_count` is correctly mapped from your **GATT DB** header.

---

## Example Log Output

```
[INFO] Bluetooth Booted. Starting Advertising...
[INFO] BLE advertising started
[INFO] Buttons initialized on PB1 and PC1
[INFO] Restart count incremented: 5
[INFO] Application initialized
[INFO] BLE connected. Handle: 0
[INFO] Notifications ENABLED by client
[INFO] Restart count notified: 5
[INFO] Both buttons pressed. Resetting restart count...
[INFO] Restart count reset to 0
```

---

