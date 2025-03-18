# BLE LED GATT service example



---
## About BLE LED GATT service example
---
This example demonstrates the controlling of LED 1 on the dev board using the BLE GATT services. The LED CONTROL BLE GATT characteristics has the write property to which a user can write '0x00' to OFF the LED and '0x01' to ON the LED. 

---
## Module compatibility
---
1. B1BMBG22M1
2. B1BMBG22M2
3. B1BMBG22M3
4. B1BMBG22M4
5. B1BMBG22M5
6. B1BMBG22M6
---
## Description
---
1. Use Si Connect Mobile appliaction as a client to use this example.
2. You will find the device advertising with the local name "Blinky".
3. Using the Si Connect mobile App connect to this deivce and you will find the LED CONROL Gatt charcateristics with write property.
4. You can write the value "0x00" to OFF the LED 1 and write "0x01" to ON the LED 1.
5. The LED GPIO mapping is given below:

    | LEDs | GPIOs  | M1/M2/M3 | M4/M5/M6
    | :---: | :---: | :---: | :---: |
    | LED 1 | PB02 | Enabled | Enabled |
    | LED 2 | PC04 | Enabled | Enabled |
    | LED 3 | PD00 | Disabled | Enabled |
    | LED 4 | PB00 | Enabled | Enabled |