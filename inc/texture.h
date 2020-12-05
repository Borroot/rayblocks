#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <SDL2/SDL.h>

typedef struct
{
	SDL_Texture *img;  /* actual texture */
	size_t w;   /* width  of the texture */
	size_t h;   /* height of the texture */
} Texture;

extern Texture texture_sky;
extern Texture texture_walls[];

void texture_init(SDL_Renderer *renderer);
void texture_quit();

#endif /* TEXTURE_H_ */
