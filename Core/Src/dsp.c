/*
 * fir.h
 *
 *  Created on: July 31, 2022
 *      Author: IPikin
 */

#include "dsp.h"
#include "audio.h"
#include "fir.h"

void Dsp_Process(void)
{
  if (Audio_FirstBufferHalfReady)
  {
    Fir_Process(&Audio_CircularBuffer[0]);
    Audio_OnFirstBufferHalfProcessed();
  }
  if (Audio_SecondBufferHalfReady)
  {
    Fir_Process(&Audio_CircularBuffer[AUDIO_CIRCULAR_BUFFER_HALF_SIZE]);
    Audio_OnSecondBufferHalfProcessed();
  }
}
