#include <SDL2/SDL.h>
#include "config.h"
#include "collide.h"
#include "color.h"
#include "point.h"
#include "state.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static void render_line(SDL_Renderer *renderer, State *state, size_t x)
{
	float camera_x = 2 * x / (double)SCREEN_WIDTH - 1;  /* x-coord on plane */
	PointF ray = {state->dir.x + state->plane.x * camera_x,
		state->dir.y + state->plane.y * camera_x};  /* ray direction */

	PointI cell = {state->pos.x, state->pos.y};  /* to be: cell hit in map */
	int side;  /* what side was hit of the wall x = 0 or y = 1 */
	float wall_dst = collide_ray(state, &ray, &cell, &side);  /* perp dist */

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
