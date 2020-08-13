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

#ifndef MENUVIEW_HPP
#define MENUVIEW_HPP

#include "models/menumodel.hpp"
#include "widgets/dataview.hpp"
#include "types.hpp"
#include "utility.hpp"

#include <string>


class Canvas;

class MenuView : public DataView<std::string>
{
    using Super = DataView<std::string>;
    
public:
    MenuView(Canvas* parent = nullptr, std::string const& id = "") noexcept;
    MenuView(SDL_Rect const&, Canvas* parent = nullptr, std::string const& id = "") noexcept;
    
private:
    virtual void render_item(Renderer const&, std::string const& item_text, SDL_Rect const& bounds) const override;
};


inline MenuView::MenuView(Canvas* parent, std::string const& id) noexcept : MenuView({0, 0, 0, 0}, parent, id) {}
inline MenuView::MenuView(SDL_Rect const& dimensions, Canvas* parent, std::string const& id) noexcept : Super(dimensions, parent, id) {}

inline void MenuView::render_item(Renderer const& renderer, std::string const& item_text, SDL_Rect const& bounds) const
{
    draw_text(renderer, bounds, this->m_item_font.lock(), item_text);
}


#endif
