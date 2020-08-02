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

#include "textitem.hpp"


/// GUI functions:
bool TextItem::render(Renderer const& renderer) const
{
    if (!WidgetItem::render(renderer))
        return false;
    
    draw_text(renderer, m_dimensions, m_font.lock(), m_text, m_alignment);
    return true;
}

/// convenience functions:
void TextItem::swap(TextItem& item) noexcept
{
    WidgetItem::swap(item);
    swap_members(item);
}

void TextItem::swap_members(TextItem& item) noexcept
{
    using std::swap;
    swap(m_font, item.m_font);
    swap(m_text, item.m_text);
    swap(m_alignment, item.m_alignment);
}
