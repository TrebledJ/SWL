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

#include "rectitem.hpp"
#include "themes.hpp"
#include "utility.hpp"


RectItem::RectItem(SDL_Rect const& dimensions, Canvas* parent, std::string const& id) noexcept
    : Super(dimensions, parent, id)
    , m_background_color{Themes::PRIMARY}
{
}

bool RectItem::render(Renderer const& renderer) const
{
    if (!Super::render(renderer))
        return false;
    
    draw_filled_rect(renderer, m_dimensions, m_enabled ? m_background_color : Colors::darken(m_background_color, 0.5));
    return true;
}
