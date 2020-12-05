#include <SDL2/SDL_ttf.h>
#include "debug.h"
#include "font.h"

TTF_Font *font_fps;  /* exported in font.h */

void font_init()
{
	font_fps = TTF_OpenFont(FONT_FILE_FPS, FONT_SIZE_FPS);
	TTF_ERROR_IF(font_fps == NULL, "font_fps coult not be created.");
}

void font_quit()
{
	TTF_CloseFont(font_fps);
}
