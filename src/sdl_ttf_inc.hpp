#ifndef SDL_TTF_INC_H
#define SDL_TTF_INC_H


#if defined(__linux__)
  #include <SDL2/SDL_ttf.h>
#elif defined(__APPLE__)
  #include <SDL2_ttf/SDL_ttf.h>
#else
  #include <SDL_ttf.h>
#endif


#endif
