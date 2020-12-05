#include <SDL2/SDL_ttf.h>
#include "config.h"
#include "debug.h"
#include "font.h"

TTF_Font *font_fps;

static void font_init_one(const char *filename, int size, TTF_Font **font)
{
	*font = TTF_OpenFont(filename, size);
	TTF_ERROR_IF(font == NULL, "Font could not be created.");
}

void font_init()
{
	font_init_one("res/fonts/SansMono-Medium.ttf", FPS_FONTSIZE, &font_fps);
}

void font_quit()
{
	TTF_CloseFont(font_fps);
}
