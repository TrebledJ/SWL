/*
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

#ifndef THEMES_H
#define THEMES_H

#include "sdl_inc.hpp"


namespace Colors
{
    /**
     * Changes these colours or add your own ones here!
     */
    const auto BLACK            = SDL_Color{0, 0, 0, 255};
    const auto BLUE             = SDL_Color{0, 0, 255, 255};
    const auto CYAN             = SDL_Color{0, 255, 255, 255};
    const auto GREEN            = SDL_Color{0, 128, 0, 255};
    const auto LIGHT_BLUE       = SDL_Color{0, 162, 232, 255};
    const auto LIGHT_GREEN      = SDL_Color{0, 255, 0, 255};
    const auto LIGHT_GREY       = SDL_Color{204, 204, 204, 255};
    const auto MAGENTA          = SDL_Color{255, 0, 255, 255};
    const auto ORANGE           = SDL_Color{230, 98, 10, 255};
    const auto PURPLE           = SDL_Color{128, 0, 128, 255};
    const auto RED              = SDL_Color{255, 0, 0, 255};
    const auto YELLOW           = SDL_Color{255, 255, 0, 255};
    const auto WHITE            = SDL_Color{255, 255, 255, 255};
    
    const auto TRANSPARENT      = SDL_Color{0, 0, 0, 0};
    
    /**
     * @param   factor A value between 0 to 1. The closer to 1, the darker the color
     */
    inline SDL_Color darken(SDL_Color const& color, float factor = 0.25);
    
    /**
     * @param   factor A value between 0 to 1. The closer to 1, the brighter the color
     */
    inline SDL_Color lighten(SDL_Color const& color, float factor = 0.25);
}

namespace Themes
{
    /**
     * Change these themes or define your own ones here!
     */
    const auto BACKGROUND = Colors::BLACK;
    const auto PRIMARY = Colors::BLUE;
    const auto SECONDARY = Colors::YELLOW;
}

namespace Colors {
    /**
     * @param   factor A value between 0 to 1. The closer to 1, the darker the color
     */
    inline SDL_Color darken(SDL_Color const& color, float factor)
    {
        factor = (factor > 1.0 ? 1.0 : factor < 0.0 ? 0.0);
        return SDL_Color{Uint8(color.r * (1 - factor)), Uint8(color.g * (1 - factor)), Uint8(color.b * (1 - factor)), color.a};
    }
    
    /**
     * @param   factor A value between 0 to 1. The closer to 1, the brighter the color
     */
    inline SDL_Color lighten(SDL_Color const& color, float factor)
    {
        factor = (factor > 1.0 ? 1.0 : factor < 0.0 ? 0.0);
        return SDL_Color{
            Uint8(color.r + (255 - color.r)*factor),
            Uint8(color.g + (255 - color.g)*factor),
            Uint8(color.b + (255 - color.b)*factor),
            color.a
        };
    }
}


#endif /* themes_h */
