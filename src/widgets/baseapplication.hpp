/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#ifndef BASEAPPLICATION_HPP
#define BASEAPPLICATION_HPP

#include "canvas.hpp"
#include "utility.hpp"
#include <string>

#include "sdl_mixer_inc.hpp"


/**
 * @brief   Manages core SDL initialisation and destruction.
 */
class BaseApplication : public Canvas
{
public:
    /// constructors:
    BaseApplication(int width, int height);
    BaseApplication(BaseApplication const&) = delete;
    BaseApplication(BaseApplication&&) = delete;
    
    /// destructor:
    virtual ~BaseApplication();
    
    /// assignment:
    BaseApplication& operator= (BaseApplication const&) = delete;
    BaseApplication& operator= (BaseApplication&&) = delete;
    
private:
    using Canvas::custom_redraw;
};


#endif
