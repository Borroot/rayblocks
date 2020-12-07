#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#include "level.h"
#include "point.h"

void level_quit(Level *level)
{
	free(level->map_obstacles);
	free(level->map_walls);
	free(level->map_floor);
	free(level->map_ceiling);
	free(level);
}

static int *read_map(FILE *file, size_t width, size_t height)
{
	int *map = malloc(width * height * sizeof(int));
	ERROR_IF(map == NULL, "map_walls pointer could not be created.");

	int current;
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			fscanf(file, "%d", &current);
			map[width * y + x] = current;
		}
	}
	return map;
}

static void skipline(FILE *file)
{
	char c;
	do {
		c = fgetc(file);
	} while (c != '\n');
}

Level *level_load(const char *filename)
{
	FILE *file = fopen(filename, "r");
	ERROR_IF(file == NULL, "Level file could not be opened.");

	Level *level = malloc(sizeof(Level));
	ERROR_IF(level == NULL, "Level pointer could not be created.");

	size_t width, height;
	PointI start, finish;

	fscanf(file, "%zu %zu", &width, &height);
	fscanf(file, "%d %d %d %d", &start.x, &start.y, &finish.x, &finish.y);

	skipline(file);

	level->map_obstacles = read_map(file, width, height); skipline(file);
	level->map_walls     = read_map(file, width, height); skipline(file);
	level->map_floor     = read_map(file, width, height); skipline(file);
	level->map_ceiling   = read_map(file, width, height);

	fclose(file);

	level->w = width;
	level->h = height;

	level->start  = start;
	level->finish = finish;

	return level;
}
