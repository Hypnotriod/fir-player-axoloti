/*
 * audio.c
 *
 *  Created on: Sep 9, 2021
 *      Author: IPikin
 */

#include <string.h>
#include "adau1961.h"
#include "audio.h"

//************ public variables ************ //
int32_t Audio_CircularBuffer[AUDIO_CIRCULAR_BUFFER_SIZE] = { 0 };
volatile bool Audio_SecondBufferHalfReady = false;
volatile bool Audio_FirstBufferHalfReady = false;

//************ private functions prototypes ************ //
void Audio_FullTransferCompleteCallback(struct __DMA_HandleTypeDef * hdma);
void Audio_HalfTransferCompleteCallback(struct __DMA_HandleTypeDef * hdma);

void Audio_Init(void)
{
  ADAU1961_Init(AUDIO_SAMPLE_RATE, true);
}

void Audio_Start(void)
{
  HAL_SAI_Receive_DMA(&hsai_BlockB1, (uint8_t*) Audio_CircularBuffer, AUDIO_CIRCULAR_BUFFER_SIZE);
  HAL_SAI_Transmit_DMA(&hsai_BlockA1, (uint8_t*) Audio_CircularBuffer, AUDIO_CIRCULAR_BUFFER_SIZE);

  hsai_BlockB1.hdmarx->XferCpltCallback = Audio_FullTransferCompleteCallback;
  hsai_BlockB1.hdmarx->XferHalfCpltCallback = Audio_HalfTransferCompleteCallback;

  hsai_BlockA1.hdmatx->XferCpltCallback = NULL;
  hsai_BlockA1.hdmatx->XferHalfCpltCallback = NULL;
}

void Audio_ToLeftMono(int32_t * pBuff)
{
  int i;
  
  for (i = 0; i < AUDIO_CIRCULAR_BUFFER_HALF_SIZE; i += AUDIO_CHANNELS_NUM)
  {
    pBuff[i + 1] = pBuff[i];
  }
}

void Audio_ToRightMono(int32_t * pBuff)
{
  int i;
  
  for (i = 0; i < AUDIO_CIRCULAR_BUFFER_HALF_SIZE; i += AUDIO_CHANNELS_NUM)
  {
    pBuff[i] = pBuff[i + 1];
  }
}

void Audio_Mute(void)
{
  memset(Audio_CircularBuffer, 0, sizeof(Audio_CircularBuffer));
}

void Audio_MuteInput(void)
{
  ADAU1961_MuteInput();
}

void Audio_UnmuteInput(void)
{
  ADAU1961_UnmuteInput();
}

void Audio_FullTransferCompleteCallback(struct __DMA_HandleTypeDef * hdma)
{
  Audio_SecondBufferHalfReady = true;
}

void Audio_HalfTransferCompleteCallback(struct __DMA_HandleTypeDef * hdma)
{
  Audio_FirstBufferHalfReady = true;
}
