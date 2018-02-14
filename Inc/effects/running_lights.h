/*
 * running_lights.h
 *
 *  Created on: 13 лют. 2018 р.
 *      Author: MarS
 */

#ifndef EFFECTS_RUNNING_LIGHTS_H_
#define EFFECTS_RUNNING_LIGHTS_H_

#include <stdlib.h>
#include <stdint.h>

#include "bitmap/bitmap.h"

typedef struct RunningLightsCtx {
	int16_t hue;
	int16_t value;
	uint8_t valueDirection;
	uint8_t gradation;

	uint16_t iteration;

	uint8_t delay;
	uint8_t delayTarget;
} RunningLightsCtx;

void running_lights(RunningLightsCtx* ctx, HSV_t *leds, uint16_t size);

#endif /* EFFECTS_RUNNING_LIGHTS_H_ */
