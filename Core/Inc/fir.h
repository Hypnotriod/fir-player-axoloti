/*
 * fir.h
 *
 *  Created on: Sep 15, 2021
 *      Author: IPikin
 */

#ifndef INC_FIR_H_
#define INC_FIR_H_

#include "audio.h"

#define FIR_TAPS_NUM        1764
#define FIR_BLOCK_SIZE      AUDIO_CIRCULAR_BUFFER_QUARTER_SIZE
#define FIR_HEADROOM_POW_2  2

void Fir_Init(void);
void Fir_LoadImpulse(const int16_t * pImpulse);
void Fir_Process(int32_t * pBuff);

#endif /* INC_FIR_H_ */
