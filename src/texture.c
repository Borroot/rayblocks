#include <SDL2/SDL.h>
#include <stdint.h>
#include "config.h"
#include "debug.h"
#include "texture.h"

#define FOLDER  "res/textures/"
#define WALLS   FOLDER "walls/"
#define FLOORS  FOLDER "floors/"

/* texture files for the walls */
const char *wallnames[] = {
	WALLS "Stone1L.bmp",        WALLS "Stone1D.bmp",
	WALLS "TilesL.bmp",         WALLS "TilesD.bmp",
	WALLS "MultibrickL.bmp",    WALLS "MultibrickD.bmp",
	WALLS "BrownbrickL.bmp",    WALLS "BrownbrickD.bmp",
	WALLS "BookshelfL.bmp",     WALLS "BookshelfD.bmp",
	WALLS "StoneUWHitlerL.bmp", WALLS "StoneUWHitlerD.bmp"};

/* texture files for the floors */
const char *floornames[] = {
	FLOORS "Hexbrown1.bmp", FLOORS "Hexbrown2.bmp", FLOORS "Hexbrown3.bmp",
	FLOORS "Woodherring2.bmp"};

Texture texture_walls[sizeof(wallnames) / sizeof(wallnames[0])];
Texture texture_sky;
Texture texture_floors[sizeof(floornames) / sizeof(floornames[0])];

static void texture_init_one(SDL_Renderer *renderer, const char *filename,
		Texture *texture, int keepsurface)
{
	SDL_Surface *surface = SDL_LoadBMP(filename);
	SDL_ERROR_IF(surface == NULL, "Texture surface could not be created.");

	texture->img = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_ERROR_IF(texture == NULL, "Texture object could not be created.");

	texture->w = surface->w;
	texture->h = surface->h;

	if (keepsurface) {
		surface = SDL_ConvertSurfaceFormat(surface, PIXELFORMAT, 0);
		texture->surf = surface;
	} else {
		texture->surf = NULL;
		SDL_FreeSurface(surface);
	}
}

void texture_init(SDL_Renderer *renderer)
{
	for (size_t i = 0; i < sizeof(wallnames) / sizeof(wallnames[0]); i++)
		texture_init_one(renderer, wallnames[i], texture_walls + i, 0);

	texture_init_one(renderer, FOLDER "skies/sky.bmp", &texture_sky, 0);

	for (size_t i = 0; i < sizeof(floornames) / sizeof(floornames[0]); i++)
		texture_init_one(renderer, floornames[i], texture_floors + i, 1);
}

void texture_quit()
{
	for (size_t i = 0; i < sizeof(wallnames) / sizeof(wallnames[0]); i++)
		SDL_DestroyTexture(texture_walls[i].img);

	SDL_DestroyTexture(texture_sky.img);

	for (size_t i = 0; i < sizeof(floornames) / sizeof(floornames[0]); i++) {
		SDL_DestroyTexture(texture_floors[i].img);
		SDL_FreeSurface(texture_floors[i].surf);
	}
}
