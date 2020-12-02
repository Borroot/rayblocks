#include <SDL2/SDL.h>
#include <stddef.h>
#include <stdlib.h>
#include "clock.h"

Clock *clock_init()
{
	Clock *clock = malloc(sizeof(Clock));
	clock->index = 0;
	for (size_t i = 0; i < SIZE_TICKS; i++) clock->ticks[i] = 0;
	return clock;
}

void clock_tick(Clock *clock)
{
	clock->index = (clock->index + 1) % SIZE_TICKS;
	clock->ticks[clock->index] = SDL_GetTicks();
}

size_t clock_dt(Clock *clock)
{
	return clock->ticks[clock->index]-clock->ticks[(clock->index+1)%SIZE_TICKS];
}

size_t clock_fps(Clock *clock)
{
	return (int)(SIZE_TICKS / (clock_dt(clock) / 1000.f));
}
