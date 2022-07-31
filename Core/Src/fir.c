/*
 * fir.c
 *
 *  Created on: Sep 15, 2021
 *      Author: IPikin
 */

#include <string.h>
#include "ccmram.h"
#include "fir.h"
#include "stm32f4xx_hal.h"
#include "arm_math.h"

static q31_t firCoeffs[FIR_TAPS_NUM] CCM_RAM_SECTION;
static q31_t firState[FIR_BLOCK_SIZE + FIR_TAPS_NUM] CCM_RAM_SECTION;
static arm_fir_instance_q31 firInstance CCM_RAM_SECTION;

void Fir_Init(void)
{
  arm_fir_init_q31(&firInstance, FIR_TAPS_NUM, firCoeffs, firState, FIR_BLOCK_SIZE);
}

void Fir_LoadImpulse(const int16_t * pImpulse)
{
  int i;
  for (i = 0; i < FIR_TAPS_NUM; i++)
  {
    firCoeffs[i] = pImpulse[i] << (16 - FIR_HEADROOM_POW_2);
  }
  memset(firState, 0, sizeof(firState));
}

void Fir_Process(int32_t * pBuff)
{
  int i;
  
  q31_t input[FIR_BLOCK_SIZE];
  q31_t output[FIR_BLOCK_SIZE];
  
  for (i = 0; i < FIR_BLOCK_SIZE; i++)
  {
    input[i] = pBuff[i * AUDIO_CHANNELS_NUM];
  }
  
  arm_fir_fast_q31(&firInstance, input, output, FIR_BLOCK_SIZE);
  
  for (i = 0; i < FIR_BLOCK_SIZE; i++)
  {
    pBuff[i * AUDIO_CHANNELS_NUM] = pBuff[i * AUDIO_CHANNELS_NUM + 1] = output[i];
  }
}
