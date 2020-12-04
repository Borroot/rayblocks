#include <SDL2/SDL.h>
#include <stdlib.h>
#include "config.h"
#include "color.h"
#include "point.h"
#include "state.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define SIGN(x)   ((x) < 0 ? -1 : 1)

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
		if (state->level->map[state->level->w * cell->y + cell->x])
			hit = 1;
	}
	return side;
}

static void render_line(SDL_Renderer *renderer, State *state, size_t x)
{
	float camera_x = 2 * x / (double)SCREEN_WIDTH - 1;  /* x-coord in camera plane */
	PointF ray = {state->dir.x + state->plane.x * camera_x,
		state->dir.y + state->plane.y * camera_x};  /* ray direction */

	PointI cell = {state->pos.x, state->pos.y};  /* current cell in map */

	PointI step = {SIGN(ray.x), SIGN(ray.y)};  /* step to next cell in map */
	PointF delt_dst = {fabs(1 / ray.x), fabs(1 / ray.y)};  /* dst to next cell */
	PointF side_dst;  /* dst to first border for x and y */

	if (ray.x < 0) side_dst.x = (state->pos.x - cell.x)       * delt_dst.x;
	else           side_dst.x = (cell.x + 1.0 - state->pos.x) * delt_dst.x;
	if (ray.y < 0) side_dst.y = (state->pos.y - cell.y)       * delt_dst.y;
	else           side_dst.y = (cell.y + 1.0 - state->pos.y) * delt_dst.y;

	int side = dda(state, &side_dst, &delt_dst, &step, &cell); /* hit: x = 0, y = 1 */
	float wall_dst;  /* distance from camera plane to the hit wall */

	if (side) wall_dst = (cell.y - state->pos.y + (1 - step.y) / 2) / ray.y;
	else      wall_dst = (cell.x - state->pos.x + (1 - step.x) / 2) / ray.x;

	int line_height = (int)(SCREEN_HEIGHT / wall_dst);
	int line_top = MAX(0, -line_height / 2 + SCREEN_HEIGHT / 2);
	int line_bot = MIN(SCREEN_HEIGHT - 1, line_height / 2 + SCREEN_HEIGHT / 2);

	SDL_Color color;
	switch (state->level->map[state->level->w * cell.y + cell.x]) {
		case 1:  color = COLOR_RED;    break;
		case 2:  color = COLOR_GREEN;  break;
		case 3:  color = COLOR_WHITE;  break;
		case 4:  color = COLOR_BLUE;   break;
		default: color = COLOR_YELLOW; break;
	}
	if (side) { color.r /= 2; color.g /= 2; color.b /= 2; }

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, x, line_top, x, line_bot);
}

void render(SDL_Renderer *renderer, State *state)
{
	for (size_t x = 0; x < SCREEN_WIDTH; x++)
		render_line(renderer, state, x);
}
