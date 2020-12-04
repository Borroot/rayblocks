#ifndef LEVEL_H_
#define LEVEL_H_

#include <stddef.h>
#include "point.h"

typedef struct {
	int **map;      /* the actual map containing cell info */
	size_t w;       /* width  of the map */
	size_t h;       /* height of the map */

	PointI start;   /* starting  cell of the level */
	PointI finish;  /* finishing cell of the level */
} Level;

Level *level_load(const char *file);
Level *level_generate(size_t width, size_t height);

#endif
