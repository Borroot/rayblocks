#include <SDL2/SDL.h>
#include "config.h"
#include "move.h"
#include "point.h"
#include "state.h"

static void event_move(State *state, float dt)
{
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);
	if (keystates[KEY_MOVE_NORTH]) /* move north */
		move(state, dt, MOVE_NORTH);
	if (keystates[KEY_MOVE_SOUTH]) /* move south */
		move(state, dt, MOVE_SOUTH);

	if (keystates[KEY_MOVE_WEST])  /* move west */
		move(state, dt, MOVE_WEST);
	if (keystates[KEY_MOVE_EAST])  /* move east */
		move(state, dt, MOVE_EAST);

	if (keystates[KEY_TURN_WEST])  /* turn west */
		turn(state, dt, TURN_WEST);
	if (keystates[KEY_TURN_EAST])  /* turn east */
		turn(state, dt, TURN_EAST);
}

static int event_general(State *state)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			return 1;
		} else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
				case KEY_QUIT:
					return 1;
				case KEY_FLOOR:
					state->floor = !state->floor;
					break;
			}
		}
	}
	return 0;
}

int event_process(State *state, float dt)
{
	if (event_general(state)) return 1;
	event_move(state, dt);
	return 0;
}
