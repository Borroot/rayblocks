#include <SDL2/SDL.h>
#include <stdlib.h>
#include "config.h"
#include "collide.h"
#include "color.h"
#include "point.h"
#include "state.h"
#include "texture.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static void render_line_wall(SDL_Renderer *renderer, State *state, size_t x)
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
	int line_height = (int)(SCREEN_HEIGHT / wall_dst);
	int line_top = SCREEN_HEIGHT / 2 - line_height / 2;

	SDL_Rect srcrect = {texture_x, 0, 1, texture.h};
	SDL_Rect dstrect = {x, line_top, 1, line_height};
	SDL_RenderCopy(renderer, texture.img, &srcrect, &dstrect);
}

static void render_line_floor(SDL_Renderer *renderer, State *state, size_t y)
{
	/* create the left and right most rays */
	PointF rayl = {state->dir.x-state->plane.x, state->dir.y-state->plane.y};
	PointF rayr = {state->dir.x+state->plane.x, state->dir.y+state->plane.y};

	int p = y - SCREEN_HEIGHT / 2;  /* height distance from ray to horizon */
	float posz = SCREEN_HEIGHT / 2.f;  /* height of the camera */
	float dst = posz / p;  /* horizontal distance to the floor */

	/* create the stepping vector which will increase the floor vector */
	PointF step = {dst * (rayr.x - rayl.x) / SCREEN_WIDTH,
		dst * (rayr.y - rayl.y) / SCREEN_WIDTH};
	PointF floor = {state->pos.x + dst * rayl.x, state->pos.y + dst * rayl.y};

	for (size_t x = 0; x < SCREEN_WIDTH; x++) {
		PointI cell = {floor.x, floor.y};

		size_t index = abs(state->level->map[state->level->w*cell.y+cell.x]);
		Texture texture = texture_floors[0];

		PointI texel = {texture.w*(floor.x-cell.x),texture.h*(floor.y-cell.y)};

		floor.x += step.x;
		floor.y += step.y;

		SDL_Rect srcrect = {texel.x, texel.y, 1, 1};
		SDL_Rect dstrect = {x, y, 1, 1};
		SDL_RenderCopy(renderer, texture.img, &srcrect, &dstrect);
	}
}

static void render_floor(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer,COLOR_GREY.r,COLOR_GREY.g,COLOR_GREY.b,0);
	SDL_Rect rect = {0, SCREEN_HEIGHT/2 + 1, SCREEN_WIDTH, SCREEN_HEIGHT/2};
	SDL_RenderFillRect(renderer, &rect);
}

static void render_sky(SDL_Renderer *renderer, State *state)
{

}

void render(SDL_Renderer *renderer, State *state)
{
	render_sky(renderer, state);

	#if RENDER_FLOOR
	for (size_t y = SCREEN_HEIGHT / 2 + 1; y <= SCREEN_HEIGHT; y++)
		render_line_floor(renderer, state, y);
	#else
	render_floor(renderer);
	#endif

	for (size_t x = 0; x < SCREEN_WIDTH; x++)
		render_line_wall(renderer, state, x);
}
