/*
 * fir.c
 *
 *  Created on: Sep 15, 2021
 *      Author: IPikin
 */

#include "fir.h"
#include "stm32f4xx_hal.h"
#include "impulse.h"
#include "arm_math.h"

static q31_t firCoeffs[FIR_TAPS_NUM];
static q31_t firState[FIR_BLOCK_SIZE + FIR_TAPS_NUM];
static arm_fir_instance_q31 firInstance;

void Fir_Init(void)
{
  int i;
  
  arm_fir_init_q31(&firInstance, FIR_TAPS_NUM, firCoeffs, firState, FIR_BLOCK_SIZE);
  
  for (i = 0; i < FIR_TAPS_NUM; i++)
  {
    firCoeffs[i] = ((q31_t) Impulses[0][i]) * 256 * 8;
  }
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
  
  arm_fir_q31(&firInstance, input, output, FIR_BLOCK_SIZE);
  
  for (i = 0; i < (FIR_BLOCK_SIZE * AUDIO_CHANNELS_NUM); i += AUDIO_CHANNELS_NUM)
  {
    pBuff[i] = pBuff[i + 1] = output[i / AUDIO_CHANNELS_NUM];
  }
}
