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
#include "ssd1306.h"
#include "stm32f4xx_hal.h"

static volatile int8_t encoder0Rotations = 0;
static uint8_t impulseIndex = 0;

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
    encoder0Rotations = 0;
    Fir_LoadImpulse(Impulses[impulseIndex]);
  }
}
