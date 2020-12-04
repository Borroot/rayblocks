#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "debug.h"
#include "draw.h"


static void fonts_init()
{
	font_fps = TTF_OpenFont(FILE_FONT_FPS, SIZE_FONT_FPS);
	TTF_ERROR_IF(font_fps == NULL, "font_fps coult not be created.");
}

static void fonts_quit()
{
	TTF_CloseFont(font_fps);
}

void draw_init(SDL_Window **window, SDL_Renderer **renderer)
{
	SDL_ERROR_IF(SDL_Init(SDL_INIT_VIDEO) < 0, "SDL could not initialize.");
	SDL_ERROR_IF(TTF_Init() < 0, "TTF could not initialize.");
	fonts_init();

	*window = SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_ERROR_IF(*window == NULL, "Window could not be created.");

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	SDL_ERROR_IF(*renderer == NULL, "Renderer could not be created.");
}

void draw_quit(SDL_Window *window, SDL_Renderer *renderer)
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	fonts_quit();
	TTF_Quit();
	SDL_Quit();
}

void draw_text(SDL_Renderer *renderer, TTF_Font *font, const SDL_Color *color,
		const PointI *pos, const char *text)
{
	SDL_Surface *surface = TTF_RenderText_Blended(font, text, *color);
	TTF_ERROR_IF(surface == NULL, "TTF surface could not be created.");

	SDL_Rect dstrect = {pos->x, pos->y, surface->w, surface->h};

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_ERROR_IF(texture == NULL, "TTF texture could not be created.");

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void draw_fps(SDL_Renderer *renderer, size_t fps)
{
	char text[10];
	snprintf(text, 10, "%ld", fps);

	SDL_Surface *surface = TTF_RenderText_Blended(font_fps, text, COLOR_GREEN);
	TTF_ERROR_IF(surface == NULL, "TTF surface could not be created.");

	SDL_Rect dstrect = {SCREEN_WIDTH-surface->w-5, 0, surface->w, surface->h};

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_ERROR_IF(texture == NULL, "TTF texture could not be created.");

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}
