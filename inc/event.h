#ifndef EVENT_H_
#define EVENT_H_

#include "state.h"

int event_process(State *state, int dt);  /* return if quit was called */

#endif /* EVENT_H_ */
