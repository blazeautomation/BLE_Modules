# Button interrupt LED control example



---
## About Button interrupt LED control example
---
This example demostrates the LED ON-OFF control by button interrupts. There are 4 LEDs and 4 Switches on the dev kit, each LED is controlled by a individual switch.

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
1. The LED and Switch GPIO mapping is given below:
    1. 1 LED GPIO Mapping

    | LEDs | GPIOs  | M1/M2/M3 | M4/M5/M6
    | :---: | :---: | :---: | :---: |
    | LED 1 | PB02 | Enabled | Enabled |
    | LED 2 | PC04 | Enabled | Enabled |
    | LED 3 | PD00 | Disabled | Enabled |
    | LED 4 | PB00 | Enabled | Enabled | 

    1. 2 Switch GPIO Mapping

    | Switches | GPIOs  | M1/M2/M3 | M4/M5/M6
    | :---: | :---: | :---: | :---: |
    | S 1 | PB01 | Enabled | Enabled |
    | S 2 | PC01 | Enabled | Enabled |
    | S 3 | PD01 | Disabled | Enabled |
    | S 4 | PA04 | Enabled | Enabled |

2. Here LED 1, LED 2, LED 3  and LED 4 are controlled by S 1, S 2, S 3 and S 4 respectively