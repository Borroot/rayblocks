#include <stdlib.h>
#include "point.h"
#include "state.h"

#define SIGN(x) ((x) < 0 ? -1 : 1)

static int dda(State *state, PointF *side_dst, PointF *delt_dst, PointI *step,
		PointI *cell)
{
	int side, hit = 0;
	while (!hit) {
		/* check a hit with the next map square in x or y direction */
		if (side_dst->x < side_dst->y) {
			side_dst->x += delt_dst->x;
			cell->x += step->x;
			side = 0;
		} else {
			side_dst->y += delt_dst->y;
			cell->y += step->y;
			side = 1;
		}
		if (state->level->map_walls[state->level->w*cell->y+cell->x] > 0)
			hit = 1;
	}
	return side;
}

float collide_ray(State *state, PointF *ray, PointI *cell, int *side)
{
	PointI step = {SIGN(ray->x), SIGN(ray->y)};  /* step to next cell in map */
	PointF delt_dst = {fabs(1 / ray->x), fabs(1 / ray->y)};  /* dst to next */
	PointF side_dst;  /* dst to first border for x and y */

	if (ray->x < 0) side_dst.x = (state->pos.x  - cell->x)      * delt_dst.x;
	else            side_dst.x = (cell->x + 1.0 - state->pos.x) * delt_dst.x;
	if (ray->y < 0) side_dst.y = (state->pos.y  - cell->y)      * delt_dst.y;
	else            side_dst.y = (cell->y + 1.0 - state->pos.y) * delt_dst.y;

	*side = dda(state, &side_dst, &delt_dst, &step, cell); /* x = 0, y = 1 */
	if (*side) return (cell->y - state->pos.y + (1 - step.y) / 2) / ray->y;
	else       return (cell->x - state->pos.x + (1 - step.x) / 2) / ray->x;
}

int collide_radius(State *state, PointF *pos, float radius)
{
	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++) {
			PointI newpos = {pos->x + dx * radius, pos->y + dy * radius};
			size_t index = state->level->w * newpos.y + newpos.x;
			if (state->level->map_obstacles[index] > 0)
				return 1;
		}
	}
	return 0;
}
