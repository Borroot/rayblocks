#ifndef LEVEL_H_
#define LEVEL_H_

#include <stddef.h>
#include "point.h"

typedef struct {
	int *map_obstacles;  /* map containing obstacle info */
	int *map_walls;      /* map containing wall     textures info */
	int *map_floor;      /* map containing floor    textures info */
	int *map_ceiling;    /* map containing ceiling  textures info */

	size_t w;  /* width  of the map */
	size_t h;  /* height of the map */

	PointI start;   /* starting  cell of the level */
	PointI finish;  /* finishing cell of the level */
} Level;

void   level_quit(Level *level);
Level *level_load(const char *filename);
Level *level_generate(size_t width, size_t height);

#endif
