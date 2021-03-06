#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "config.h"
#include "collide.h"
#include "color.h"
#include "debug.h"
#include "point.h"
#include "state.h"
#include "sprite.h"
#include "texture.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

SDL_Texture *floor_texture;
uint32_t *floor_pixels;

SDL_Texture *ceil_texture;
uint32_t *ceil_pixels;

int *sprite_dst;

static void render_init_floorceil(SDL_Renderer *renderer,
		SDL_Texture **texture, uint32_t **pixels)
{
	*texture = SDL_CreateTexture(renderer, PIXELFORMAT,
		SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT / 2);
	SDL_ERROR_IF(*texture == NULL, "Could not create texture.");

	int result = SDL_SetTextureBlendMode(*texture, SDL_BLENDMODE_BLEND);
	SDL_ERROR_IF(result < 0, "Could not set blend mode for texture.");

	size_t len = WIDTH * (HEIGHT / 2 + 1) * sizeof(**pixels);
	*pixels = malloc(len);
	ERROR_IF(*pixels == NULL, "Could not malloc pixels array.");
	memset(*pixels, 0, len);
}

void render_init(SDL_Renderer *renderer)
{
	render_init_floorceil(renderer, &floor_texture, &floor_pixels);
	render_init_floorceil(renderer, &ceil_texture, &ceil_pixels);
}

void render_quit()
{
	SDL_DestroyTexture(floor_texture);
	free(floor_pixels);
}

static float render_line(SDL_Renderer *renderer, State *state, size_t x)
{
	float camera_x = 2 * x / (double)WIDTH - 1;  /* x-coord on plane */
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
	int line_height = (int)(HEIGHT / wall_dst);
	int line_top = HEIGHT / 2 - line_height / 2;

	/* draw the wall texture on the screen at the correct place */
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

	/* make all floor pixels behind the walls opague */
	for (int y = HEIGHT / 2 + 1; y < MIN(line_end, HEIGHT); y++)
		floor_pixels[WIDTH * (y - HEIGHT / 2 - 1) + x] = 0;

	/* make all ceiling pixels behind the walls opague */
	for (int y = MAX(0, line_top); y < HEIGHT / 2; y++)
		ceil_pixels[WIDTH * y + x] = 0;

	/* draw the ceiling and floor pixels step by step on the screen */
	for (size_t y = line_end; y < HEIGHT + 1; y++)
	{
		float current_dst = HEIGHT / (2.0 * y - HEIGHT);
		float weight = current_dst / wall_dst;

		/* calculate the exact floor position which needs be drawn */
		PointF floor = {weight * floorwall.x + (1.0 - weight) * state->pos.x,
			            weight * floorwall.y + (1.0 - weight) * state->pos.y};

		/* retrieve the texture from the floor tile we are currently drawing */
		size_t index = state->level->w * (int)floor.y + (int)floor.x;
		size_t texnum = state->level->map_floor[index];

		floor.x = floor.x - (int)floor.x;  /* only leave the decimals */
		floor.y = floor.y - (int)floor.y;  /* only leave the decimals */

		int drawy = y - HEIGHT / 2 - 1;  /* y pos in floor pixel array */

		/* update the floor pixel to the pixel from the correct texture */
		if (texnum == 0)  /* no texture */
			floor_pixels[WIDTH * drawy + x] = 0;
		else {
			Texture *texture = &texture_floorceil[texnum - 1];
			PointI texel={(int)(floor.x*texture->w),(int)(floor.y*texture->h)};
			floor_pixels[WIDTH * drawy + x] =
				((uint32_t*)texture->surf->pixels)[texture->w*texel.y+texel.x];
		}

		/* update the ceiling pixel to the pixel from the correct texture */
		texnum = state->level->map_ceiling[index];
		if (texnum == 0)  /* no texture */
			ceil_pixels[WIDTH * (HEIGHT / 2 - drawy) + x] = 0;
		else {
			texture = &texture_floorceil[texnum - 1];
			PointI texel={(int)(floor.x*texture->w),(int)(floor.y*texture->h)};
			ceil_pixels[WIDTH * (HEIGHT / 2 - drawy) + x] =
				((uint32_t*)texture->surf->pixels)[texture->w*texel.y+texel.x];
		}
	}
	return wall_dst;  /* perp dst */
}

static int compar(const void *a, const void *b)
{
	return sprite_dst[*((size_t *)b)] - sprite_dst[*((size_t *)a)];
}

static void render_sprites(SDL_Renderer *renderer, State *state,
		float *zbuffer)
{
	size_t numsprites = state->level->numsprites;  /* abbreviation */
	Sprite *sprites = state->level->sprites;       /* abbreviation */

	/* determine the order to paint the sprites (painter's algorithm) */
	size_t *sprite_order = malloc(numsprites * sizeof(size_t));
	sprite_dst = malloc(numsprites * sizeof(int));

	for (size_t i = 0; i < numsprites; i++) {
		sprite_order[i] = i;
		sprite_dst[i] =
			(state->pos.x - sprites[i].x) * (state->pos.x - sprites[i].x) +
			(state->pos.y - sprites[i].y) * (state->pos.y - sprites[i].y);
	}
	qsort(sprite_order, numsprites, sizeof(size_t), compar);  /* sort them */

	for (size_t i = 0; i < numsprites; i++) {
		/* translate sprite position relative to the player */
		PointF sprite = {sprites[sprite_order[i]].x - state->pos.x,
		                 sprites[sprite_order[i]].y - state->pos.y};

		/* transform sprite with the inverse camera matrix */
		float invdet = 1.0 /
			(state->plane.x * state->dir.y - state->plane.y * state->dir.x);

		/* calculate the transformation values */
		float tx = invdet*( state->dir.y  *sprite.x - state->dir.x  *sprite.y);
		float ty = invdet*(-state->plane.y*sprite.x + state->plane.x*sprite.y);

		/* calculate the x value of the middle of the sprite on the screen */
		int sprite_screenx = (WIDTH / 2) * (1 + tx / ty);

		int sprite_size = fabs(HEIGHT / ty);  /* height and width (same) */
		int sprite_top = HEIGHT / 2 - sprite_size / 2;

		int sprite_left  = -sprite_size / 2 + sprite_screenx;
		int sprite_right =  sprite_size / 2 + sprite_screenx;

		for (int x = sprite_left; x < sprite_right; x++) {
			Texture *texture = sprites[sprite_order[i]].texture;
			int texture_x = (x - (-sprite_size / 2 + sprite_screenx)) *
				texture->w / sprite_size;

			if (ty > 0 && x >= 0 && x < WIDTH && ty < zbuffer[x]) {
				/* draw the sprite texture on the screen */
				SDL_Rect srcrect = {texture_x, 0, 1, texture->h};
				SDL_Rect dstrect = {x, sprite_top, 1, sprite_size};
				SDL_RenderCopy(renderer, texture->img, &srcrect, &dstrect);
			}
		}
	}

	free(sprite_order);
	free(sprite_dst);
}

static void render_wall_and_floor(SDL_Renderer *renderer, State *state)
{
	float zbuffer[WIDTH];  /* zbuffer stores wall distances */

	for (size_t x = 0; x < WIDTH; x++)
		zbuffer[x] = render_line(renderer, state, x);  /* draw vertical line */

	/* push updates for floor and ceiling to the screen from pixel arrays */
	size_t pitch1 = WIDTH * sizeof(*floor_pixels);
	size_t pitch2 = WIDTH * sizeof(*ceil_pixels);

	SDL_UpdateTexture(floor_texture, NULL, floor_pixels, pitch1);
	SDL_UpdateTexture(ceil_texture, NULL, ceil_pixels, pitch2);

	SDL_Rect dstrect1 = {0, HEIGHT / 2, WIDTH, HEIGHT / 2};
	SDL_Rect dstrect2 = {0, 0, WIDTH, HEIGHT / 2};

	SDL_RenderCopy(renderer, floor_texture, NULL, &dstrect1);
	SDL_RenderCopy(renderer, ceil_texture, NULL, &dstrect2);

	render_sprites(renderer, state, zbuffer);  /* draw all the sprites */
}

static void render_sky(SDL_Renderer *renderer, State *state)
{
	Texture *texture = &texture_sky;

	size_t texture_width = (2 * texture->h * WIDTH) / HEIGHT;
	ERROR_IF(texture_width > texture->w, "Sky texture is not wide enough.");

	float angle = 2 * M_PI - (atan2f(state->dir.x, state->dir.y) + M_PI);
	size_t offset = angle / (2 * M_PI) * texture->w;  /* offset in texture */

	if (offset + texture_width < texture->w) {  /* no cycling is needed */
		SDL_Rect srcrect = {offset, 0, texture_width, texture->h};
		SDL_Rect dstrect = {0, 0, WIDTH, HEIGHT / 2};
		SDL_RenderCopy(renderer, texture->img, &srcrect, &dstrect);
	} else {  /* the texture needs to wrap around */
		size_t width1 = texture->w - offset;
		size_t width2 = texture_width - width1;

		size_t screen_width1 = width1/(float)texture_width * WIDTH;
		size_t screen_width2 = width2/(float)texture_width * WIDTH + 1;

		SDL_Rect srcrect1 = {offset, 0, width1, texture->h};
		SDL_Rect dstrect1 = {0, 0, screen_width1, HEIGHT / 2};
		SDL_RenderCopy(renderer, texture->img, &srcrect1, &dstrect1);

		SDL_Rect srcrect2 = {0, 0, width2, texture->h};
		SDL_Rect dstrect2 = {screen_width1, 0, screen_width2, HEIGHT / 2};
		SDL_RenderCopy(renderer, texture->img, &srcrect2, &dstrect2);
	}
}

void render(SDL_Renderer *renderer, State *state)
{
	render_sky(renderer, state);
	render_wall_and_floor(renderer, state);
}
