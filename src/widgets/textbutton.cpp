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

#include "widgets/textbutton.hpp"

#include "themes.hpp"
#include "utility.hpp"


/// GUI functions:
bool TextButton::handle_mouse_event(MouseEvent const& event)
{
    //  filter events by relying on base function
    if (!Super::handle_mouse_event(event))
        return false;
    
    ButtonInterface::handle(event);
    return true;
}

void TextButton::render(Renderer const& renderer) const
{
    Super::render(renderer);    //  draw button before text
    draw_text(renderer, m_dimensions, m_font.lock(), text(), m_alignment);
}

