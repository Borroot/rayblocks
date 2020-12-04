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

	Clock *clock = clock_init();
	size_t fps = 0;

	Level *level = level_load("res/maps/map2.txt");
	State *state = state_init(level);

	int quit = 0;
	while (!quit) {
		quit = event_process(state, clock_dt(clock));
		clock_tick(clock);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		render(renderer, state);

		if (!(clock->count % CONF_FPS_DRAWRATE)) fps = clock_fps(clock);
		draw_fps(renderer, fps);

		SDL_RenderPresent(renderer);
	}

	free(clock);
	state_quit(state);
	draw_quit(window, renderer);

	return 0;
}
