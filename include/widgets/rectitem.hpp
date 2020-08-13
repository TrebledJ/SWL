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

#ifndef RECTITEM_HPP
#define RECTITEM_HPP

#include "widgetitem.hpp"
#include "types.hpp"

#include "sdl_inc.hpp"


/**
 * @brief   Displays a block of color. Useful for backgrounds.
 */
class RectItem : public WidgetItem
{
public:
    /// constructors:
    RectItem(Canvas* parent = nullptr, std::string const& id = "") noexcept;
    RectItem(SDL_Rect const& dimensions, Canvas* parent = nullptr, std::string const& id = "") noexcept;
    RectItem(RectItem const&) = delete;
    RectItem(RectItem&&) = delete;
    
    /// destructor:
    virtual ~RectItem();
    
    /// assignment:
    RectItem& operator= (RectItem const&) = delete;
    RectItem& operator= (RectItem&&) = delete;
    
    /// modifiers:
    void background(SDL_Color const& color);
    
    /// GUI functions:
    virtual void render(Renderer const& renderer) const override;
    
protected:
    SDL_Color m_background_color;
};


/// constructors:
inline RectItem::RectItem(Canvas* parent, std::string const& id) noexcept : RectItem({0, 0, 0, 0}, parent, id) {}

/// destructor:
inline RectItem::~RectItem() = default;

/// modifiers:
inline void RectItem::background(SDL_Color const& color)
{
    m_background_color = color;
}


#endif
