#ifndef CLOCK_H_
#define CLOCK_H_

#include <stddef.h>

#define SIZE_TICKS 100

typedef struct
{
	size_t ticks[SIZE_TICKS];
	size_t index;  /* index of the previous tick */
} Clock;

Clock *clock_init();
void   clock_tick(Clock *clock);
size_t clock_dt(Clock *clock);
size_t clock_fps(Clock *clock);

#endif /* CLOCK_H_ */
