# BLE Modules and Dev board



---
## About BLE Modules and Dev board
---
BLE modules and dev board comproses of a base board with 4 LEDs and Buttons and two slot options, in 1st slot B1BMBG22M1,B1BMBG22M2 and B1BMBG22M3 are allowed. In the 2nd slot B1BMBG22M4,B1BMBG22M5 and B1BMBG22M6 are allowed, the dev kit can be used with any of these 2 types of modules with its 3 different variants each. The 3 variants comprises of EFR32BG22C112F352GM32-C,EFR32BG22C222F352GM32-C and EFR32BG22C224F512GM32-C respectively.


---
## Specifications
---
1. B1BMBG22M1 and B1BMBG22M4 is having the SoC chip EFR32BG22C112F352GM32-C
2. B1BMBG22M2 and B1BMBG22M5 is having the SoC chip EFR32BG22C222F352GM32-C
3. B1BMBG22M3 and B1BMBG22M6 is having the SoC chip EFR32BG22C224F512GM32-C
4. The Module GPIO Pinout description for B1BMBG22M1/B1BMBG22M2/B1BMBG22M3 are as follows:

    | Pinout | Description    |
    | :---: | :---: |
    | Pin 1 | VCC | 
    | Pin 2 | PA05(Txd) | 
    | Pin 3 | PA06(Rxd) | 
    | Pin 4 | PB00 |
    | Pin 5 | RESET |
    | Pin 6 | PA03 |
    | Pin 7 | PA04 |
    | Pin 8 | GND |
    | Pin 9 | PC01 |
    | Pin 10 | PC02 |
    | Pin 11 | PA01 (SW CLK) |
    | Pin 12 | PA02 (SWDIO) |
    | Pin 13 | PC03 |
    | Pin 14 | PC04 |
    | Pin 15 | PB01 |
    | Pin 16 | PB02 |

5. The Module GPIO Pinout description for B1BMBG22M4/B1BMBG22M5/B1BMBG22M6

    | Pinout | Description    |
    | :---: | :---: |
    | Pin 1 | Ext Antenne | 
    | Pin 2 | PB02 | 
    | Pin 3 | PB01 | 
    | Pin 4 | PB00 |
    | Pin 5 | PA00 |
    | Pin 6 | PA03 |
    | Pin 7 | PA04 |
    | Pin 8 | PA05 (Txd) |
    | Pin 9 | PA06 (Rxd) |
    | Pin 10 | GND |
    | Pin 11 | RESET |
    | Pin 12 | PA01 (SW CLK) |
    | Pin 13 | PA02 (SWDIO) |
    | Pin 14 | GND |
    | Pin 15 | VCC |
    | Pin 16 | PD01 |
    | Pin 17 | PD00 |
    | Pin 18 | PC00 |
    | Pin 19 | PC01 |
    | Pin 20 | PC02 |
    | Pin 21 | PC03 |
    | Pin 22 | PC04 |
    | Pin 23 | PC05 |
    | Pin 24 | GND |

6. The LEDs and switches GPIO pinouts for the dev kit base board is given below for both B1BMBG22M1/B1BMBG22M2/B1BMBG22M3 and B1BMBG22M4/B1BMBG22M5/B1BMBG22M6:

    6. 1  LED
    
    | LEDs | GPIOs  | M1/M2/M3 | M4/M5/M6
    | :---: | :---: | :---: | :---: |
    | LED 1 | PB02 | Enabled | Enabled |
    | LED 2 | PC04 | Enabled | Enabled |
    | LED 3 | PD00 | Disabled | Enabled |
    | LED 4 | PB00 | Enabled | Enabled |

    6. 2  Button

    | Switches | GPIOs  | M1/M2/M3 | M4/M5/M6
    | :---: | :---: | :---: | :---: |
    | S 1 | PB01 | Enabled | Enabled |
    | S 2 | PC01 | Enabled | Enabled |
    | S 3 | PD01 | Disabled | Enabled |
    | S 4 | PA04 | Enabled | Enabled |