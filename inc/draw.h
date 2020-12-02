#ifndef DRAW_H_
#define DRAW_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stddef.h>

#define FILE_FONT1 "res/fonts/PublicSans-SemiBold.ttf"
#define SIZE_FONT1 20
static TTF_Font *font1;

static const SDL_Color COLOR_BLACK = {  0,   0,   0,   0};
static const SDL_Color COLOR_WHITE = {255, 255, 255,   0};

void draw_init(SDL_Window **window, SDL_Renderer **renderer);
void draw_quit(SDL_Window *window, SDL_Renderer *renderer);

void draw_text(SDL_Renderer *renderer, TTF_Font *font, const SDL_Color *color,
		const SDL_Point *pos, const char *text);
void draw_fps(SDL_Renderer *renderer, size_t fps);

#endif /* DRAW_H_ */
