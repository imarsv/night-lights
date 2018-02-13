/*
 * running_lights.c
 *
 *  Created on: 13 лют. 2018 р.
 *      Author: MarS
 */

#include <stdio.h>

#include <effects/running_lights.h>

#define MAX_VALUE (128 + 64 + 32)
#define MIN_VALUE (64)
#define GRADATION_COUNT_VALUE (5)

void running_lights(RunningLightsCtx* ctx, HSV_t *leds, uint16_t size) {

	for (int i = 0; i < size - 1; i++) {
		leds[size - 1 - i] = leds[size - 1 - i - 1];
	}

	if (ctx->iteration == 0) {
		ctx->hue = rand() % 360;
		ctx->value = MIN_VALUE;
		ctx->valueDirection = 1;
		ctx->gradation = 1;
		ctx->gradationTarget = 1 + rand() % 6;
	}

	leds[0].h = ctx->hue;
	leds[0].v = ctx->value;
	leds[0].s = 128;

	if (ctx->valueDirection) {
		ctx->value += (MAX_VALUE - MIN_VALUE) / ctx->gradation;
	} else {
		ctx->value -= (MAX_VALUE - MIN_VALUE) / ctx->gradation;
	}

	if (ctx->valueDirection == 1 && ctx->value >= MAX_VALUE) {
		ctx->valueDirection = 0;
	}

	if (ctx->valueDirection == 0 && ctx->value < MIN_VALUE) {
		ctx->hue = rand() % 360;
		ctx->value = MIN_VALUE;
		ctx->valueDirection = 1;
	}

//	printf("iteration: %-4d  delay: %-2d=>%-2d  hue: %-3d  value: %-3d  gradation: %-2d=>%-2d \n ",
//			ctx->iteration,
//			ctx->delay, ctx->delayTarget,
//			ctx->hue, ctx->value,
//			ctx->gradation, ctx->gradationTarget);

	if (ctx->iteration % (size / 5) == 0) {
		if (ctx->delay == ctx->delayTarget) {
			ctx->delayTarget = 15 + rand() % 50;
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

	if (ctx->iteration % (size * 2) == 0) {
		if (ctx->gradation > ctx->gradationTarget) {
			ctx->gradation--;
		} else if (ctx->gradation < ctx->gradationTarget) {
			ctx->gradation++;
		}

		if (ctx->gradation == ctx->gradationTarget) {
			ctx->gradationTarget = 1 + rand() % 6;
		}
	}

	ctx->iteration++;
}
