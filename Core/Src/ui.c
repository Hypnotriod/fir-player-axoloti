/*
 * ui.h
 *
 *  Created on: Jul 23, 2022
 *      Author: IPikin
 */
 
#include "ui.h"
#include "main.h"
#include "impulse.h"
#include "fir.h"
#include "dsp.h"
#include "audio.h"
#include "ssd1306.h"
#include "stm32f4xx_hal.h"

static volatile int8_t encoder0Rotations = 0;
static uint8_t impulseIndex = 0;

const uint8_t speakerIcon[] = // 32x30
{
	0x78, 0x3F, 0xF0, 0x78, //  ####     ##########     #### 
	0xF8, 0xFF, 0xFC, 0x7C, // #####   ##############   #####
	0xFF, 0xE0, 0x1F, 0xFC, // ###########        ###########
	0xFF, 0x80, 0x07, 0xFC, // #########            #########
	0xFE, 0x07, 0x81, 0xFC, // #######      ####      #######
	0x3C, 0x3F, 0xF0, 0xF0, //   ####    ##########    ####  
	0x38, 0x7C, 0xF8, 0x70, //   ###    #####  #####    ###  
	0x31, 0xE0, 0x1E, 0x30, //   ##   ####        ####   ##  
	0x71, 0x80, 0x06, 0x38, //  ###   ##            ##   ### 
	0x63, 0x00, 0x03, 0x18, //  ##   ##              ##   ## 
	0xE7, 0x00, 0x03, 0x9C, // ###  ###              ###  ###
	0xC6, 0x00, 0x01, 0x8C, // ##   ##                ##   ##
	0xC6, 0x07, 0x81, 0x8C, // ##   ##      ####      ##   ##
	0xCE, 0x0F, 0xC1, 0xCC, // ##  ###     ######     ###  ##
	0xCC, 0x0F, 0xC0, 0xCC, // ##  ##      ######      ##  ##
	0xCC, 0x0F, 0xC0, 0xCC, // ##  ##      ######      ##  ##
	0xCE, 0x0F, 0xC1, 0xCC, // ##  ###     ######     ###  ##
	0xC6, 0x07, 0x81, 0x8C, // ##   ##      ####      ##   ##
	0xC6, 0x00, 0x01, 0x8C, // ##   ##                ##   ##
	0xE7, 0x00, 0x03, 0x9C, // ###  ###              ###  ###
	0x63, 0x00, 0x03, 0x18, //  ##   ##              ##   ## 
	0x71, 0x80, 0x06, 0x38, //  ###   ##            ##   ### 
	0x31, 0xE0, 0x1E, 0x30, //   ##   ####        ####   ##  
	0x38, 0x7C, 0xF8, 0x70, //   ###    #####  #####    ###  
	0x3C, 0x3F, 0xF0, 0xF0, //   ####    ##########    ####  
	0xFE, 0x07, 0x81, 0xFC, // #######      ####      #######
	0xFF, 0x80, 0x07, 0xFC, // #########            #########
	0xFF, 0xE0, 0x1F, 0xFC, // ###########        ###########
	0xF8, 0xFF, 0xFC, 0x7C, // #####   ##############   #####
	0x78, 0x3F, 0xF0, 0x78, //  ####     ##########     #### 
};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == ENC0_A_INT_Pin)
  {
    encoder0Rotations += (ENC0_B_GPIO_Port->IDR & ENC0_B_Pin) ? 1 : -1;
  }
}

void UI_Update(void)
{
  if (encoder0Rotations)
  {
    if (encoder0Rotations > 0)
    {
      impulseIndex = (impulseIndex + 1) % IMPULSES_NUM;
    }
    else
    {
      impulseIndex = (IMPULSES_NUM + impulseIndex - 1) % IMPULSES_NUM;
    }
    Audio_MuteInput();
    Audio_Mute();
    UI_LoadImpulse();
    Audio_OnFirstBufferHalfProcessed();
    while (!Audio_FirstBufferHalfReady) {};
    Audio_OnSecondBufferHalfProcessed();
    Dsp_Process();
    Audio_UnmuteInput();
    encoder0Rotations = 0;
  }
}

void UI_LoadImpulse(void)
{
  Fir_LoadImpulse(Impulses[impulseIndex].impulse);
  SSD1306_Fill(Black);
  SSD1306_DrawBitmap(0, 0, speakerIcon, 32, 30, White);
  SSD1306_SetCursor(0, 32);
  SSD1306_WriteString(Impulses[impulseIndex].vendor, Font_11x18, White);
  SSD1306_SetCursor(0, 52);
  SSD1306_WriteString(Impulses[impulseIndex].description, Font_6x8, White);
  SSD1306_UpdateScreen();
}
