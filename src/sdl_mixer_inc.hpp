#ifndef SDL_MIXER_INC_H
#define SDL_MIXER_INC_H


#if defined(__linux__)
  #include <SDL2/SDL_mixer.h>
#elif defined(__APPLE__)
  #include <SDL2_mixer/SDL_mixer.h>
#else
  #include <SDL_mixer.h>
#endif


#endif
