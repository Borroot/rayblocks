#include <math.h>
#include "point.h"

void point_rotate(PointF *point, float angle)
{
	float oldx = point->x, oldy = point->y;  /* multiply by rotation matrix */
	point->x = oldx * cosf(angle) - oldy * sinf(angle);
	point->y = oldx * sinf(angle) + oldy * cosf(angle);
}
