#include <SDL2/SDL.h>
#include "debug.h"
#include "texture.h"

#define FOLDER  "res/textures/"
#define WALLS   FOLDER "walls/"
#define FLOORS  FOLDER "floors/"

const char *wallnames[] = {
	WALLS "Stone1L.bmp",        WALLS "Stone1D.bmp",
	WALLS "TilesL.bmp",         WALLS "TilesD.bmp",
	WALLS "MultibrickL.bmp",    WALLS "MultibrickD.bmp",
	WALLS "BrownbrickL.bmp",    WALLS "BrownbrickD.bmp",
	WALLS "StoneUWHitlerL.bmp", WALLS "StoneUWHitlerD.bmp"};

const char *floornames[] = {
	FLOORS "Hexbrown1.bmp", FLOORS "Hexbrown2.bmp", FLOORS "Hexbrown3.bmp",
	FLOORS "Woodherring2.bmp"};

Texture texture_walls[sizeof(wallnames) / sizeof(wallnames[0])];
Texture texture_sky;
Texture texture_floors[sizeof(floornames) / sizeof(floornames[0])];

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
	for (size_t i = 0; i < sizeof(wallnames) / sizeof(wallnames[0]); i++)
		texture_init_one(renderer, wallnames[i], texture_walls + i);

	texture_init_one(renderer, FOLDER "sky.bmp", &texture_sky);

	for (size_t i = 0; i < sizeof(floornames) / sizeof(floornames[0]); i++)
		texture_init_one(renderer, floornames[i], texture_floors + i);
}

void texture_quit()
{
	for (size_t i = 0; i < sizeof(wallnames) / sizeof(wallnames[0]); i++)
		SDL_DestroyTexture(texture_walls[i].img);

	SDL_DestroyTexture(texture_sky.img);

	for (size_t i = 0; i < sizeof(floornames) / sizeof(floornames[0]); i++)
		SDL_DestroyTexture(texture_floors[i].img);
}
