/*
 * rainbow.h
 *
 *  Created on: 13 лют. 2018 р.
 *      Author: MarS
 */

#ifndef EFFECTS_RAINBOW_H_
#define EFFECTS_RAINBOW_H_

#include <stdint.h>
#include "bitmap/bitmap.h"

typedef struct RainbowCtx {
  int16_t hue;

  uint8_t value;
  uint8_t valueTarget;

  uint8_t delay;
  uint8_t delayTarget;
} RainbowCtx;

void rainbow(RainbowCtx* ctx, uint8_t mode, HSV_t *leds, uint16_t size);

#endif /* EFFECTS_RAINBOW_H_ */
