#include <SDL2/SDL.h>
#include <stdint.h>
#include "config.h"
#include "debug.h"
#include "texture.h"

#define SKIES     "res/textures/skies/"
#define WALLS     "res/textures/walls/"
#define FLOORCEIL "res/textures/floorceil/"
#define SPRITES   "res/textures/sprites/"

/* texture files for the floor and the ceiling */
const char *floorceilnames[] = {
	FLOORCEIL "Hexbrown1.bmp",    FLOORCEIL "Hexbrown2.bmp",
	FLOORCEIL "Hexbrown3.bmp",    FLOORCEIL "Woodherring1.bmp",
	FLOORCEIL "Woodherring2.bmp"};

/* texture files for the walls */
const char *wallnames[] = {
	WALLS "Stone1L.bmp",        WALLS "Stone1D.bmp",
	WALLS "TilesL.bmp",         WALLS "TilesD.bmp",
	WALLS "MultibrickL.bmp",    WALLS "MultibrickD.bmp",
	WALLS "BrownbrickL.bmp",    WALLS "BrownbrickD.bmp",
	WALLS "BookshelfL.bmp",     WALLS "BookshelfD.bmp"};

/* texture files for the sprites */
const char *spritenames[] = {
	SPRITES "barrel1.bmp", SPRITES "snowman1.bmp", SPRITES "xmastree.bmp"};

Texture texture_sky;
Texture texture_walls[sizeof(wallnames) / sizeof(*wallnames)];
Texture texture_floorceil[sizeof(floorceilnames) / sizeof(*floorceilnames)];
Texture texture_sprites[sizeof(spritenames) / sizeof(*spritenames)];

static void texture_init_one(SDL_Renderer *renderer, const char *filename,
		Texture *texture, int keepsurface)
{
	SDL_Surface *surface = SDL_LoadBMP(filename);
	SDL_ERROR_IF(surface == NULL, "Texture surface could not be created.");

	texture->img = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_ERROR_IF(texture->img == NULL, "Texture object could not be created.");

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
	texture_init_one(renderer, SKIES "sky.bmp", &texture_sky, 0);

	for (size_t i = 0; i < sizeof(wallnames) / sizeof(*wallnames); i++)
		texture_init_one(renderer, wallnames[i], texture_walls + i, 0);

	for (size_t i = 0; i < sizeof(floorceilnames)/sizeof(*floorceilnames); i++)
		texture_init_one(renderer, floorceilnames[i], texture_floorceil+i, 1);

	for (size_t i = 0; i < sizeof(spritenames) / sizeof(*spritenames); i++) {
		texture_init_one(renderer, spritenames[i], texture_sprites + i, 0);
		SDL_SetTextureBlendMode(texture_sprites[i].img, SDL_BLENDMODE_BLEND);
	}
}

void texture_quit()
{
	SDL_DestroyTexture(texture_sky.img);

	for (size_t i = 0; i < sizeof(wallnames) / sizeof(*wallnames); i++)
		SDL_DestroyTexture(texture_walls[i].img);

	for (size_t i = 0; i<sizeof(floorceilnames)/sizeof(*floorceilnames); i++) {
		SDL_DestroyTexture(texture_floorceil[i].img);
		SDL_FreeSurface(texture_floorceil[i].surf);
	}

	for (size_t i = 0; i < sizeof(spritenames) / sizeof(*spritenames); i++)
		SDL_DestroyTexture(texture_sprites[i].img);
}
