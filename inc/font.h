#ifndef FONT_H_
#define FONT_H_

#include "config.h"

#define FONT_FILE_FPS "res/fonts/NotoSansMono-Medium.ttf"
#define FONT_SIZE_FPS FPS_FONTSIZE
extern TTF_Font *font_fps;  /* declared in font.c */

void font_init();
void font_quit();

#endif /* FONT_H_ */
