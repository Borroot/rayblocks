#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include "clock.h"
#include "config.h"
#include "draw.h"
#include "event.h"
#include "level.h"
#include "render.h"
#include "state.h"

int main(int argc, char **args)
{
	SDL_Window *window = NULL; SDL_Renderer *renderer = NULL;
	draw_init(&window, &renderer);

	size_t lasttime = SDL_GetTicks();  /* last time the fps was drawn */
	size_t fps = 0;
	Clock *clock = clock_init();

	State *state = state_init(level_load("res/maps/map2.txt"));

	int quit = 0;
	while (!quit) {
		/* UPDATE */
		quit = event_process(state, clock_dt(clock));
		clock_tick(clock);

		if (SDL_GetTicks() - lasttime > FPS_DRAWRATE) {
			lasttime = SDL_GetTicks();
			fps = clock_fps(clock);
		}

		/* DRAW */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		render(renderer, state);
		draw_fps(renderer, fps);

		SDL_RenderPresent(renderer);
	}

	free(clock);
	state_quit(state);
	draw_quit(window, renderer);

	return 0;
}
