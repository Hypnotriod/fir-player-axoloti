# fir-player-axoloti

FIR player implementation example based on the [Axoloti](https://cdm.link/axoloti-makes-any-music-hardware-you-can-imagine/) board and [CMSIS-DSP](https://arm-software.github.io/CMSIS-DSP/v1.16.2/group__FIR.html) library  
Includes impulses from the [Bass IR Pack](https://shift-line.com/irpackbass)

## Hardware
* EC11 20 Detent Rotary Encoder Module (1 pulse per detent)
* SSD1306 OLED Display Module (128 x 64)

## Connection
* Encoder A -> PA0
* Encoder B -> PA1
* Encoder Button -> PA2
* SSD1306 CS -> PA4
* SSD1306 SCK -> PA5
* SSD1306 DC -> PA6
* SSD1306 MOSI -> PA7
* SSD1306 Reset -> PB0
