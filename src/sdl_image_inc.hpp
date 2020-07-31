#ifndef SDL_IMAGE_INC_H
#define SDL_IMAGE_INC_H


#if defined(__linux__)
  #include <SDL2/SDL_image.h>
#elif defined(__APPLE__)
  #include <SDL2_image/SDL_image.h>
#else
  #include <SDL_image.h>
#endif


#endif
