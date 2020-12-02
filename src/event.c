#include <SDL2/SDL.h>

void event_process(SDL_bool *quit)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			*quit = SDL_TRUE;
		} else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
				case SDLK_q:
					*quit = SDL_TRUE;
					break;
			}
		}
	}
}
