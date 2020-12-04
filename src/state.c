#include <math.h>
#include "config.h"
#include "level.h"
#include "point.h"
#include "state.h"

#include <stdio.h>

State *state_init(Level *level)
{
	State *state = malloc(sizeof(State));
	PointF pos   = {level->start.x, level->start.y};
	PointF dir   = {1, 0};
	PointF plane = {0, atan((FOV / 2.0) * M_PI / 180.0)};

	state->level = level;
	state->pos = pos;
	state->dir = dir;
	state->plane = plane;
	return state;
}
