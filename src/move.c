#include <math.h>
#include "config.h"
#include "collide.h"
#include "move.h"
#include "point.h"
#include "state.h"

static void move_correctly(State *state, PointF *newpos)
{
	int xisfree = 0, yisfree = 0;
	PointF checkpos;

	checkpos.x = newpos->x;    checkpos.y = state->pos.y;
	if (!collide_radius(state, &checkpos, PLAYER_RADIUS)) xisfree = 1;
	checkpos.x = state->pos.x; checkpos.y = newpos->y;
	if (!collide_radius(state, &checkpos, PLAYER_RADIUS)) yisfree = 1;

	if (xisfree) state->pos.x = newpos->x;
	if (yisfree) state->pos.y = newpos->y;
}

static void turn_exactly(State *state, float angle)
{
	point_rotate(&state->dir,   angle);
	point_rotate(&state->plane, angle);
}

void move(State *state, float dt, const int MOVE)
{
	float speed = dt * SPEED_MOVE / 1000.f;

	PointF *pos = &state->pos;
	PointF *dir = &state->dir;

	if (MOVE == MOVE_NORTH) {
		PointF newpos = {pos->x + dir->x * speed, pos->y + dir->y * speed};
		move_correctly(state, &newpos);
	}
	if (MOVE == MOVE_SOUTH) {
		PointF newpos = {pos->x - dir->x * speed, pos->y - dir->y * speed};
		move_correctly(state, &newpos);
	}
	if (MOVE == MOVE_WEST) {
		turn_exactly(state, -M_PI_2);
		move(state, dt, MOVE_NORTH);
		turn_exactly(state,  M_PI_2);
	}
	if (MOVE == MOVE_EAST) {
		turn_exactly(state,  M_PI_2);
		move(state, dt, MOVE_NORTH);
		turn_exactly(state, -M_PI_2);
	}
}

void turn(State *state, float dt, const int TURN)
{
	float angle = dt * SPEED_TURN / 1000.f;
	if (TURN == TURN_WEST) {
		turn_exactly(state, -angle);
	} else { /* TURN_EAST */
		turn_exactly(state, angle);
	}
}
