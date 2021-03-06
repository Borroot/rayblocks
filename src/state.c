#include <stdlib.h>
#include <math.h>
#include "config.h"
#include "debug.h"
#include "level.h"
#include "point.h"
#include "state.h"

State *state_init(Level *level)
{
	State *state = malloc(sizeof(State));
	ERROR_IF(state == NULL, "State pointer could not be created.");

	PointF pos   = {level->start.x + 0.5, level->start.y + 0.5};
	PointF dir   = {1, 0};
	PointF plane = {0, atan((FOV / 2.0) * M_PI / 180.0)};

	state->level = level;
	state->pos = pos;
	state->dir = dir;
	state->plane = plane;
	return state;
}

void state_quit(State *state)
{
	level_quit(state->level);
	free(state);
}
