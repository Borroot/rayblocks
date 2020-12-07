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

static void render_line(SDL_Renderer *renderer, State *state, size_t x)
{
	float camera_x = 2 * x / (double)SCREEN_WIDTH - 1;  /* x-coord on plane */
	PointF ray = {state->dir.x + state->plane.x * camera_x,
		state->dir.y + state->plane.y * camera_x};  /* ray direction */

	PointI cell = {state->pos.x, state->pos.y};  /* to be: cell hit in map */
	int side;  /* what side was hit of the wall x = 0 or y = 1 */
	float wall_dst = collide_ray(state, &ray, &cell, &side);  /* perp dist */

	size_t index = state->level->w * cell.y + cell.x;
	size_t texnum = (state->level->map_walls[index] - 1) * 2;
	if (!side) texnum++;  /* choose dark texture variant */
	Texture *texture = &texture_walls[texnum];

	float wall_hit;  /* coordinate at which the wall is hit */
	if (side) wall_hit = state->pos.x + wall_dst * ray.x;
	else      wall_hit = state->pos.y + wall_dst * ray.y;
	wall_hit -= (int)wall_hit;

	/* calculate the texture and screen coordinates where to draw */
	int texture_x = (int)(wall_hit * (float)texture->w);
	int line_height = (int)(SCREEN_HEIGHT / wall_dst);
	int line_top = SCREEN_HEIGHT / 2 - line_height / 2;

	/* draw the texture on the screen at the correct place */
	SDL_Rect srcrect = {texture_x, 0, 1, texture->h};
	SDL_Rect dstrect = {x, line_top, 1, line_height};
	SDL_RenderCopy(renderer, texture->img, &srcrect, &dstrect);

	/* render everything below this wall as floor */
	PointF floorwall;  /* the floor below the wall which was hit */
	if (side) {
		floorwall.x = cell.x + wall_hit;
		floorwall.y = cell.y + (ray.y < 0 ? 1.0 : 0);
	} else {
		floorwall.x = cell.x + (ray.x < 0 ? 1.0 : 0);
		floorwall.y = cell.y + wall_hit;
	}

	int line_end = line_top + line_height + 1;

	/* make all pixels behind the walls opague */
	for (int y = SCREEN_HEIGHT / 2 + 1; y < MIN(line_end,SCREEN_HEIGHT); y++)
		floor_pixels[SCREEN_WIDTH * (y - SCREEN_HEIGHT / 2 - 1) + x] = 0;

	/* draw the floor pixels per y value starting from where the wall stops */
	for (size_t y = line_end; y < SCREEN_HEIGHT; y++)
	{
		float current_dst = SCREEN_HEIGHT / (2.0 * y - SCREEN_HEIGHT);
		float weight = current_dst / wall_dst;

		/* calculate the exact floor position which needs be drawn */
		PointF floor = {weight * floorwall.x + (1.0 - weight) * state->pos.x,
			            weight * floorwall.y + (1.0 - weight) * state->pos.y};

		/* retrieve the texture from the floor tile we are currently drawing */
		size_t index = state->level->w * (int)floor.y + (int)floor.x;
		size_t texnum = state->level->map_floor[index];
		Texture *texture = &texture_floors[texnum];

		/* calculate the texture pixel (texel) which is to be drawn at (x,y) */
		PointI texel = {(int)(floor.x * texture->w) % texture->w,
			            (int)(floor.y * texture->h) % texture->h};

		/* update the pixel in the pixel array to the pixel from the texture */
		floor_pixels[SCREEN_WIDTH * (y - SCREEN_HEIGHT / 2 - 1) + x] =
			((uint32_t*)texture->surf->pixels)[texture->w * texel.y + texel.x];
	}
}

static void render_wall_and_floor(SDL_Renderer *renderer, State *state)
{
	for (size_t x = 0; x < SCREEN_WIDTH; x++)
		render_line(renderer, state, x);  /* update every vertical line */

	/* push the updates for the floor to the screen from the pixel array */
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

		size_t screen_width1 = width1/(float)texture_width * SCREEN_WIDTH;
		size_t screen_width2 = width2/(float)texture_width * SCREEN_WIDTH + 1;

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
	render_wall_and_floor(renderer, state);
}

void render_init(SDL_Renderer *renderer)
{
	floor_texture = SDL_CreateTexture(renderer, PIXELFORMAT,
		SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT / 2);
	SDL_ERROR_IF(floor_texture == NULL, "Could not create floor_texture.");

	int result = SDL_SetTextureBlendMode(floor_texture, SDL_BLENDMODE_BLEND);
	SDL_ERROR_IF(result < 0, "Could not set blend mode for floor_texture.");

	size_t len = SCREEN_WIDTH * (SCREEN_HEIGHT/2 + 1) * sizeof(*floor_pixels);
	floor_pixels = malloc(len);
	ERROR_IF(floor_pixels == NULL, "Could not malloc floor_pixels array.");
}

void render_quit()
{
	SDL_DestroyTexture(floor_texture);
	free(floor_pixels);
}
