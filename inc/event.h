#ifndef EVENT_H_
#define EVENT_H_

#define MOVE_NORTH 0
#define MOVE_EAST  1
#define MOVE_WEST  2
#define MOVE_SOUTH 3

#define TURN_EAST  4
#define TURN_WEST  5

void event_process(SDL_bool *quit);

#endif /* EVENT_H_ */
