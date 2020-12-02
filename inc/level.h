#ifndef LEVEL_H_
#define LEVEL_H_

#include <stddef.h>

void   level_draw(size_t width, size_t height, int **map);
int  **level_load(const char *file);
int  **level_generate(size_t width, size_t height);

#endif
