#include <SDL2/SDL.h>
#include <stdio.h>
#include "config.h"
#include "point.h"
#include "state.h"

static void event_move(State *state, float dt)
{
	float move = dt * SPEED_MOVE / 1000.f;
	float turn = dt * SPEED_TURN / 1000.f;

	const Uint8 *keystates = SDL_GetKeyboardState(NULL);
	/* TODO Improve collision detection. */
	if (keystates[SDL_SCANCODE_W]) { /* move north */
		if (!state->level->map[state->level->w * (int)state->pos.y + (int)(state->pos.x + state->dir.x * move)])
			state->pos.x += state->dir.x * move;
		if (!state->level->map[state->level->w * (int)(state->pos.y + state->dir.y * move) + (int)state->pos.x])
			state->pos.y += state->dir.y * move;
	}
	if (keystates[SDL_SCANCODE_S]) { /* move south */
		if (!state->level->map[state->level->w * (int)state->pos.y + (int)(state->pos.x - state->dir.x * move)])
			state->pos.x -= state->dir.x * move;
		if (!state->level->map[state->level->w * (int)(state->pos.y - state->dir.y * move) + (int)state->pos.x])
			state->pos.y -= state->dir.y * move;
	}

	if (keystates[SDL_SCANCODE_A])  /* move west */
		; /* TODO */
	if (keystates[SDL_SCANCODE_D])  /* move east */
		; /* TODO */

	if (keystates[SDL_SCANCODE_J]) {  /* turn west */
		point_rotate(&state->dir,   -turn);
		point_rotate(&state->plane, -turn);
	}
	if (keystates[SDL_SCANCODE_K]) { /* turn east */
		point_rotate(&state->dir,   turn);
		point_rotate(&state->plane, turn);
	}
}

int event_process(State *state, float dt)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			return 1;
		} else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
				case SDLK_q:
					return 1;
			}
		}
	}
	event_move(state, dt);
	return 0;
}
