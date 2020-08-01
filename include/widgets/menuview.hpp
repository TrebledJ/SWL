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

#ifndef MENUVIEW_HPP
#define MENUVIEW_HPP

#include "menumodel.hpp"
#include "dataview.hpp"
#include "textbutton.hpp"
#include <string>


class MenuView : public DataView<std::string>
{
public:
    MenuView() noexcept;
    MenuView(SDL_Rect const&) noexcept;
    
private:
    virtual void render_item(Renderer const&, std::string const& item_text, SDL_Rect const& bounds) const override;
};


inline MenuView::MenuView() noexcept : MenuView({0, 0, 0, 0}) {}
inline MenuView::MenuView(SDL_Rect const& dimensions) noexcept : DataView<std::string>(dimensions) {}

inline void MenuView::render_item(Renderer const& renderer, std::string const& item_text, SDL_Rect const& bounds) const
{
    draw_text(renderer, bounds, this->m_item_font.lock(), item_text);
}


#endif
