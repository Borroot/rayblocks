#include <SDL2/SDL.h>
#include "config.h"
#include "collide.h"
#include "color.h"
#include "point.h"
#include "state.h"
#include "texture.h"

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

	size_t index = (state->level->map[state->level->w * cell.y + cell.x]-1)*2;
	if (!side) index++;  /* choose dark texture variant */
	Texture texture = texture_walls[index];

	float wall_hit;  /* coordinate at which the wall is hit */
	if (side) wall_hit = state->pos.x + wall_dst * ray.x;
	else      wall_hit = state->pos.y + wall_dst * ray.y;

	int texture_x = (int)((wall_hit - (int)wall_hit) * (float)texture.w);
	if(side == 0 && ray.x > 0) texture_x = texture.w - texture_x - 1;
	if(side == 1 && ray.y < 0) texture_x = texture.w - texture_x - 1;

	int line_height = (int)(SCREEN_HEIGHT / wall_dst);
	int line_top = SCREEN_HEIGHT / 2 - line_height / 2;

	SDL_Rect srcrect = {texture_x, 0, 1, texture.h};
	SDL_Rect dstrect = {x, line_top, 1, line_height};
	SDL_RenderCopy(renderer, texture.img, &srcrect, &dstrect);
}

static void render_floor(SDL_Renderer *renderer, State *state)
{

}

static void render_sky(SDL_Renderer *renderer, State *state)
{

}

void render(SDL_Renderer *renderer, State *state)
{
	render_floor(renderer, state);
	render_sky(renderer, state);

	for (size_t x = 0; x < SCREEN_WIDTH; x++)
		render_line(renderer, state, x);
}
