#ifndef STATE_H_
#define STATE_H_

#include "level.h"
#include "point.h"

typedef struct
{
	Level *level;  /* the current level that is being played */
	PointF pos;    /* current position of the player */
	PointF dir;    /* direction the player is looking into as vector */
	PointF plane;  /* right point of the camera plane relative to dir */

} State;

State *state_init(Level *level);
void   state_quit(State *state);

#endif /* STATE_H_ */
