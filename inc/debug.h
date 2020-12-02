#ifndef DEBUG_H_
#define DEBUG_H_

#include <SDL2/SDL.h>

#define SDL_ERROR_IF(condition, msg)                                          \
	if (condition) {                                                          \
		SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "%s %s.", msg, SDL_GetError()); \
	}

#define TTF_ERROR_IF(condition, msg)                                          \
	if (condition) {                                                          \
		SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "%s %s.", msg, TTF_GetError()); \
	}

#endif /* DEBUG_H_ */
