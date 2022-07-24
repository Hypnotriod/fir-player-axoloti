
#ifndef __IMPULSE_H__
#define __IMPULSE_H__

#include "stm32f4xx_hal.h"

#define IMPULSE_SAMPLES_NUM 4410
#define IMPULSE_SIZE        (IMPULSE_SAMPLES_NUM * 2)
#define IMPULSES_NUM        5

extern const int16_t * Impulses[IMPULSES_NUM];

#endif
