#ifndef CLOCK_H_
#define CLOCK_H_

#include <stddef.h>
#include "config.h"

#define SIZE_TICKS FPS_TICKSIZE

typedef struct
{
	size_t ticks[SIZE_TICKS];  /* list of times at which a tick occured */
	size_t index;              /* index of the previous tick */
	size_t count;              /* total number of ticks */
} Clock;

Clock *clock_init();
void   clock_tick(Clock *clock);
float  clock_dt(Clock *clock);
size_t clock_fps(Clock *clock);

#endif /* CLOCK_H_ */
