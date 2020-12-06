#ifndef CONFIG_H_
#define CONFIG_H_

#define SCREEN_TITLE  "Rayblocks"
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 960

#define FOV 90  /* field of view of the player */
#define PLAYER_RADIUS 0.25  /* collision radius of the player */

#define SPEED_MOVE 5    /* move speed amplifier */
#define SPEED_TURN 3.5  /* turn speed amplifier */

#define FPS_TICKSIZE 10   /* size of clock array (bigger -> more accurate) */
#define FPS_FONTSIZE 18   /* font size of fps on screen */
#define FPS_DRAWRATE 500  /* update displayed fps in ms */

#define KEY_MOVE_NORTH SDL_SCANCODE_W
#define KEY_MOVE_SOUTH SDL_SCANCODE_S
#define KEY_MOVE_WEST  SDL_SCANCODE_A
#define KEY_MOVE_EAST  SDL_SCANCODE_D
#define KEY_TURN_WEST  SDL_SCANCODE_J
#define KEY_TURN_EAST  SDL_SCANCODE_K

#define KEY_QUIT  SDLK_q  /* quit the program */
#define KEY_FLOOR SDLK_f  /* toggle floor rendering */

#define PIXELFORMAT SDL_PIXELFORMAT_ARGB8888  /* pixel format for surfaces */

#endif /* CONFIG_H_ */
