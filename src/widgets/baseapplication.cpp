/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#include "baseapplication.hpp"
#include "utility.hpp"

#include "sdl_image_inc.hpp"
#include "sdl_mixer_inc.hpp"
#include "sdl_ttf_inc.hpp"
#include "sdl_inc.hpp"


/// constructor:
BaseApplication::BaseApplication(int width, int height)
    : Canvas(width, height)
{
    Util::assert_equals(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO), 0, "[ERROR] SDL_Init - ${sdl_error}");
    
    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    Util::assert_equals(IMG_Init(img_flags), img_flags, "[ERROR] IMG_Init - ${img_error}");
    
    Util::assert_equals(TTF_Init(), 0, "[ERROR] TTF_Init - ${ttf_error}");
        
    Util::assert_equals(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096), 0, "[ERROR] Mix_OpenAudio: ${mix_error}");
    Util::assert_equals(MIX_InitFlags(Mix_Init(MIX_INIT_OGG)), MIX_INIT_OGG, "[ERROR] Mix_Init: ${mix_error}");
}

/// destructor:
BaseApplication::~BaseApplication()
{
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
