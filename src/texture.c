#include <SDL2/SDL.h>
#include "debug.h"
#include "texture.h"

Texture texture_sky;

static void texture_init_one(SDL_Renderer *renderer, const char *filename,
		Texture *texture)
{
	SDL_Surface *surface = SDL_LoadBMP(filename);
	SDL_ERROR_IF(surface == NULL, "Texture surface could not be created.");

	texture->img = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_ERROR_IF(texture == NULL, "Texture object could not be created.");

	texture->w = surface->w;
	texture->h = surface->h;

	SDL_FreeSurface(surface);
}

void texture_init(SDL_Renderer *renderer)
{
	texture_init_one(renderer, "res/textures/sky.bmp", &texture_sky);
}

void texture_quit()
{
	SDL_DestroyTexture(texture_sky.img);
}
