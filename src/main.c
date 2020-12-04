#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#include "clock.h"
#include "config.h"
#include "debug.h"
#include "draw.h"
#include "event.h"
#include "level.h"
#include "state.h"

int main(int argc, char **args)
{
	SDL_Window *window = NULL; SDL_Renderer *renderer = NULL;
	draw_init(&window, &renderer);

	Clock *clock = clock_init();
	size_t fps = 0;

	Level *level = level_load("res/maps/map2.txt");
	State *state = state_init(level);

	SDL_bool quit = SDL_FALSE;
	while (!quit) {
		event_process(&quit);
		clock_tick(clock);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		if (!(clock->count % CONF_FPS_DRAWRATE)) fps = clock_fps(clock);
		draw_fps(renderer, fps);

		SDL_RenderPresent(renderer);
	}

	free(clock);
	state_quit(state);
	draw_quit(window, renderer);

	return 0;
}
