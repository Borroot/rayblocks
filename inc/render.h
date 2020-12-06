#ifndef RENDER_H_
#define RENDER_H_

#include <SDL2/SDL.h>
#include "state.h"

void render_init(SDL_Renderer *renderer);
void render_quit();

void render(SDL_Renderer *renderer, State *state);

#endif /* RENDER_H_ */
