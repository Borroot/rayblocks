#ifndef POINT_H_
#define POINT_H_

#include <SDL2/SDL.h>

typedef SDL_Point PointI;  /* integer 2d point */

typedef struct
{
	float x;
	float y;
} PointF;  /* float 2d point */

void point_rotate(PointF *point, float angle);

#endif /* POINT_H_ */
