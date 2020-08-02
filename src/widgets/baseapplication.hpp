/*
 *      Copyright (C) 2020 Johnathan Law
 *
 *      This file is part of SWL.
 *
 *      SWL is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      SWL is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with SWL.  If not, see <https://www.gnu.org/licenses/>.
 *
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
