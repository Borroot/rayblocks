#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "clock.h"
#include "debug.h"
#include "draw.h"
#include "event.h"


int main(int argc, char **args)
{
	SDL_Window *window = NULL; SDL_Renderer *renderer = NULL;
	draw_init(&window, &renderer);

	Clock *clock = clock_init();

	SDL_bool quit = SDL_FALSE;
	while (!quit) {
		event_process(&quit);
		clock_tick(clock);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		draw_fps(renderer, clock_fps(clock));

		SDL_RenderPresent(renderer);
	}

	free(clock);
	draw_quit(window, renderer);

	return 0;
}
