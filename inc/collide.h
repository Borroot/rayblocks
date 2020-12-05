#ifndef COLLIDE_H_
#define COLLIDE_H_

#include "point.h"
#include "state.h"

float collide_ray(State *state, PointF *ray, PointI *cell, int *side);
int collide_radius(State *state, PointF *pos, float radius);

#endif /* COLLIDE_H_ */
