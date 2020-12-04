#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#include "level.h"
#include "point.h"

Level *level_load(const char *filename)
{
	FILE *file = fopen(filename, "r");
	ERROR_IF(file == NULL, "Level file could not be opened.");

	Level *level = malloc(sizeof(Level));
	ERROR_IF(level == NULL, "Level pointer could not be created.");

	size_t width, height;
	int current;
	PointI start, finish;

	fscanf(file, "%zu %zu", &width, &height);
	fscanf(file, "%d %d %d %d", &start.x, &start.y, &finish.x, &finish.y);

	int *map = malloc(width * height * sizeof(int));
	ERROR_IF(map == NULL, "Map pointer could not be created.");

	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			fscanf(file, "%d", &current);
			map[width * y + x] = current;
		}
	}
	fclose(file);

	level->map = map;
	level->w = width;
	level->h = height;
	level->start = start;
	level->finish = finish;
	return level;
}
