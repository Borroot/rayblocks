#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL2/SDL.h>
#include "texture.h"

typedef struct
{
	Texture *texture;  /* sprite texture info */
	float x;    /* x position of the sprite on the map */
	float y;    /* y position of the sprite on the map */
} Sprite;

#endif /* SPRITE_H_ */
