/*
 * fir.h
 *
 *  Created on: Sep 15, 2021
 *      Author: IPikin
 */

#ifndef INC_FIR_H_
#define INC_FIR_H_

#include "audio.h"

#define FIR_TAPS_NUM        640
#define FIR_BLOCK_SIZE      AUDIO_CIRCULAR_BUFFER_QUARTER_SIZE
#define FIR_STATE_SIZE      FIR_TAPS_NUM + FIR_BLOCK_SIZE * 3     // https://arm-software.github.io/CMSIS-DSP/v1.15.0/group__FIR.html

void Fir_Init(void);
void Fir_LoadImpulse(const int16_t * pImpulse, int32_t size);
void Fir_Process(int32_t *pBuff);

#endif /* INC_FIR_H_ */
