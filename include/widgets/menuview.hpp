/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
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
