/*
 * audio.h
 *
 *  Created on: Sep 9, 2021
 *      Author: IPikin
 */

#ifndef INC_AUDIO_H_
#define INC_AUDIO_H_

#include "stm32f4xx_hal.h"
#include "sai.h"
#include <stdbool.h>

#define AUDIO_CIRCULAR_BUFFER_SIZE          32
#define AUDIO_CIRCULAR_BUFFER_HALF_SIZE     (AUDIO_CIRCULAR_BUFFER_SIZE / 2)
#define AUDIO_CIRCULAR_BUFFER_QUARTER_SIZE  (AUDIO_CIRCULAR_BUFFER_SIZE / 4)
#define AUDIO_SAMPLE_RATE                   48000

void Audio_Init(void);
void Audio_Start(void);

extern int32_t Audio_CircularBuffer[AUDIO_CIRCULAR_BUFFER_SIZE];
extern volatile bool Audio_SecondBufferHalfReady;
extern volatile bool Audio_FirstBufferHalfReady;

#define Audio_OnFirstBufferHalfProcessed() Audio_FirstBufferHalfReady = false
#define Audio_OnSecondBufferHalfProcessed() Audio_SecondBufferHalfReady = false

#endif /* INC_AUDIO_H_ */
