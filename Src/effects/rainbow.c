/*
 * rainbow.c
 *
 *  Created on: 13 лют. 2018 р.
 *      Author: MarS
 */

#ifndef EFFECTS_RAINBOW_C_
#define EFFECTS_RAINBOW_C_

#include <stdlib.h>
#include <stdio.h>
#include "effects/rainbow.h"

void rainbow(RainbowCtx* ctx, HSV_t *leds, uint16_t size) {

	for (int i = 0; i < size; i++) {
		leds[i].h = ctx->hue + (360 / size) * i;
		leds[i].s = 128; // 0 - light 200 - saturation
		leds[i].v = ctx->value; // 0 - dark  200 - light
	}

//	printf("hue: %-3d  value: %-3d=>%-3d  delay: %-2d=>%-2d\n",
//			ctx->hue,
//			ctx->value, ctx->valueTarget,
//			ctx->delay, ctx->delayTarget);

	if (ctx->hue % 30 == 0) {
		if (ctx->delay == ctx->delayTarget) {
			ctx->delayTarget = rand() % 25;
		}

		if (ctx->delay == 0) {
			ctx->delay = ctx->delayTarget;
		}

		if (ctx->delay > ctx->delayTarget) {
			ctx->delay--;
		} else if (ctx->delay < ctx->delayTarget) {
			ctx->delay++;
		}
	}

	if (ctx->hue % 60 == 0) {
		if (ctx->value == ctx->valueTarget) {
			ctx->valueTarget = 64 + rand() % 128;
		}

		if (ctx->value == 0) {
			ctx->value = ctx->valueTarget;
		}

		if (ctx->value > ctx->valueTarget) {
			ctx->value--;
		} else if (ctx->value < ctx->valueTarget) {
			ctx->value++;
		}
	}


	ctx->hue++;

	if (ctx->hue >= 360) {
		ctx->hue -= 360;
	}
}

#endif /* EFFECTS_RAINBOW_C_ */
