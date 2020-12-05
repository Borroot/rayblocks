#include <SDL2/SDL.h>
#include "debug.h"
#include "texture.h"

#define FOLDER "res/textures/"
#define FWALLS FOLDER "walls/"

const char *wallnames[] = {
	FWALLS "Stone1L.bmp",        FWALLS "Stone1D.bmp",
	FWALLS "TilesL.bmp",         FWALLS "TilesD.bmp",
	FWALLS "MultibrickL.bmp",    FWALLS "MultibrickD.bmp",
	FWALLS "BrownbrickL.bmp",    FWALLS "BrownbrickD.bmp",
	FWALLS "StoneUWHitlerL.bmp", FWALLS "StoneUWHitlerD.bmp"};

Texture texture_sky;
Texture texture_walls[sizeof(wallnames) / sizeof(wallnames[0])];

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
	texture_init_one(renderer, FOLDER "sky.bmp", &texture_sky);

	for (size_t i = 0; i < sizeof(wallnames) / sizeof(wallnames[0]); i++)
		texture_init_one(renderer, wallnames[i], texture_walls + i);
}

void texture_quit()
{
	SDL_DestroyTexture(texture_sky.img);
}
