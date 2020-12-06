#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "config.h"
#include "collide.h"
#include "color.h"
#include "debug.h"
#include "point.h"
#include "state.h"
#include "texture.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

SDL_Texture *floor_texture;
uint32_t *floor_pixels;

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
	Texture *texture = &texture_walls[index];

	float wall_hit;  /* coordinate at which the wall is hit */
	if (side) wall_hit = state->pos.x + wall_dst * ray.x;
	else      wall_hit = state->pos.y + wall_dst * ray.y;

	int texture_x = (int)((wall_hit - (int)wall_hit) * (float)texture->w);
	int line_height = (int)(SCREEN_HEIGHT / wall_dst);
	int line_top = SCREEN_HEIGHT / 2 - line_height / 2;

	SDL_Rect srcrect = {texture_x, 0, 1, texture->h};
	SDL_Rect dstrect = {x, line_top, 1, line_height};
	SDL_RenderCopy(renderer, texture->img, &srcrect, &dstrect);
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

	/* writing every single pixel is extremely slow (not accelerated by gpu) */
	for (size_t x = 0; x < SCREEN_WIDTH; x++) {
		PointI cell = {floor.x, floor.y};

		Texture *texture = &texture_floors[0];
		PointI texel={texture->w*(floor.x-cell.x),texture->h*(floor.y-cell.y)};

		floor.x += step.x;
		floor.y += step.y;

		texel.x = MAX(0, texel.x);
		texel.y = MAX(0, texel.y);

		floor_pixels[SCREEN_WIDTH * (y - SCREEN_HEIGHT / 2 - 1) + x] =
			((uint32_t*)texture->surf->pixels)[texture->w * texel.y + texel.x];
	}
}

static void render_floor(SDL_Renderer *renderer, State *state)
{
	for (size_t y = SCREEN_HEIGHT / 2 + 1; y <= SCREEN_HEIGHT; y++)
		render_line_floor(renderer, state, y);

	SDL_UpdateTexture(floor_texture, NULL, floor_pixels, SCREEN_WIDTH *
		sizeof(*floor_pixels));

	SDL_Rect dstrect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
	SDL_RenderCopy(renderer, floor_texture, NULL, &dstrect);
}

static void render_sky(SDL_Renderer *renderer, State *state)
{
	Texture *texture = &texture_sky;

	size_t texture_width = (2 * texture->h * SCREEN_WIDTH) / SCREEN_HEIGHT;
	ERROR_IF(texture_width > texture->w, "Sky texture not wide enough.");

	float angle = 2 * M_PI - (atan2f(state->dir.x, state->dir.y) + M_PI);
	size_t offset = angle / (2 * M_PI) * texture->w;  /* offset in texture */

	if (offset + texture_width < texture->w) {
		SDL_Rect srcrect = {offset, 0, texture_width, texture->h};
		SDL_Rect dstrect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
		SDL_RenderCopy(renderer, texture->img, &srcrect, &dstrect);
	} else {  /* the texture needs to wrap around */
		size_t width1 = texture->w - offset;
		size_t width2 = texture_width - width1;

		size_t screen_width1 = width1 / (float)texture_width * SCREEN_WIDTH;
		size_t screen_width2 = width2 / (float)texture_width * SCREEN_WIDTH;

		SDL_Rect srcrect1 = {offset, 0, width1, texture->h};
		SDL_Rect dstrect1 = {0, 0, screen_width1, SCREEN_HEIGHT/2};
		SDL_RenderCopy(renderer, texture->img, &srcrect1, &dstrect1);

		SDL_Rect srcrect2 = {0, 0, width2, texture->h};
		SDL_Rect dstrect2 = {screen_width1, 0, screen_width2, SCREEN_HEIGHT/2};
		SDL_RenderCopy(renderer, texture->img, &srcrect2, &dstrect2);
	}
}

void render(SDL_Renderer *renderer, State *state)
{
	render_sky(renderer, state);
	render_floor(renderer, state);

	for (size_t x = 0; x < SCREEN_WIDTH; x++)
		render_line_wall(renderer, state, x);
}

void render_init(SDL_Renderer *renderer)
{
	floor_texture = SDL_CreateTexture(renderer, PIXELFORMAT,
		SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT / 2);
	SDL_ERROR_IF(floor_texture == NULL, "Could not create floor_texture.");

	floor_pixels = malloc(SCREEN_WIDTH * (SCREEN_HEIGHT / 2 + 1) *
		sizeof(*floor_pixels));
	ERROR_IF(floor_pixels == NULL, "Could not malloc floor_pixels array.");
}

void render_quit()
{
	SDL_DestroyTexture(floor_texture);
	free(floor_pixels);
}
