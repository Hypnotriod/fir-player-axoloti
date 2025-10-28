
#ifndef __IMPULSE_H__
#define __IMPULSE_H__

#include "stm32f4xx_hal.h"

#define IMPULSE_SAMPLES_NUM 4410
#define IMPULSE_SIZE        (IMPULSE_SAMPLES_NUM * 2)
#define IMPULSES_NUM        17

typedef struct {
  const int16_t *impulse;
  const int32_t size;
  const char *vendor;
  const char *description;
} Impulse_t;

extern const Impulse_t Impulses[IMPULSES_NUM];

#endif
