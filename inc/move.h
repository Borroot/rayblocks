#ifndef MOVE_H_
#define MOVE_H_

#include "state.h"

#define MOVE_NORTH 0
#define MOVE_SOUTH 1
#define MOVE_WEST  2
#define MOVE_EAST  3

#define TURN_WEST  4
#define TURN_EAST  5

void move(State *state, float dt, const int MOVE);
void turn(State *state, float dt, const int TURN);

#endif /* MOVE_H_ */
