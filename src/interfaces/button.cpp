/*
 *     Copyright (C) 2020 Johnathan Law
 *
 *     This file is part of SWL.
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

#include "interfaces/button.hpp"


void ButtonInterface::handle(MouseEvent const& event) const
{
    switch (event.type)
    {
    case MouseEvent::UP:
        clicked(event);
        
        switch (event.button)
        {
        case SDL_BUTTON_LEFT:
            left_clicked(event);
            break;
        case SDL_BUTTON_RIGHT:
            right_clicked(event);
            break;
        default:
            break;
        }
        break;
        
    case MouseEvent::DOWN:
        pressed(event);
        break;
        
    case MouseEvent::MOTION:
    {
        auto lpressed = event.button & SDL_BUTTON_LMASK;
        auto rpressed = event.button & SDL_BUTTON_RMASK;
        if (!lpressed && !rpressed)
            hovered(event);
        
        break;
    }
    }
}

