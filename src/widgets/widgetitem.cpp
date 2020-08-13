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

#include "widgetitem.hpp"

#include "canvas.hpp"
#include "types.hpp"
#include "utility.hpp"


/// constructors:
WidgetItem::WidgetItem(Canvas* parent, std::string const& id) noexcept
    : WidgetItem(SDL_Rect{0}, parent, id)
{
}
WidgetItem::WidgetItem(SDL_Rect const& dimensions, Canvas* parent, std::string const& name) noexcept
    : m_dimensions(dimensions)
    , m_parent(parent)
    , m_enabled(true)
{
    if (parent)
        parent->add_item(name, this);
}

void WidgetItem::show() { if (m_parent) m_parent->show(id); }
void WidgetItem::hide() { if (m_parent) m_parent->hide(id); }


/// convenience functions:
void WidgetItem::swap(WidgetItem& item) noexcept
{
    using std::swap;
    swap(id, item.id);
    swap(m_dimensions, item.m_dimensions);
    swap(m_parent, item.m_parent);
    swap(m_enabled, item.m_enabled);
}
