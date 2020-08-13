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

#ifndef TEXTBUTTON_HPP
#define TEXTBUTTON_HPP

#include "widgets/rectitem.hpp"
#include "interfaces/button.hpp"
#include "interfaces/text.hpp"


class Canvas;

/**
 * @brief   Combines a button together with text
 *
 * @note    Handler Events: (inherited events)
 *
 * @note    Fonts will not be managed by this class.
 *          They should remain existent for the duration of the TextButton object.
 *
 * @note    Item will not be visible if font is not set.
 */
class TextButton : public RectItem, public ButtonInterface, public TextInterface
{
    using Super = RectItem;
    
public:
    /// constructors:
    TextButton(Canvas* parent = nullptr, std::string const& id = "") noexcept;
    TextButton(SDL_Rect const& dimensions, Canvas* parent = nullptr, std::string const& id = "") noexcept;
    TextButton(TextButton const&) = delete;
    TextButton(TextButton&&) = delete;
    
    /// destructor:
    virtual ~TextButton();
    
    /// assignment:
    TextButton& operator= (TextButton const&) = delete;
    TextButton& operator= (TextButton&&) = delete;
    
    /// GUI functions:
    virtual bool handle_mouse_event(MouseEvent const&) override;
    virtual void render(Renderer const&) const override;
};


/// constructors:
inline TextButton::TextButton(Canvas* parent, std::string const& id) noexcept
    : TextButton(SDL_Rect{0}, parent, id)
{
}
inline TextButton::TextButton(SDL_Rect const& dimensions, Canvas* parent, std::string const& id) noexcept
    : Super(dimensions, parent, id)
    , TextInterface(ALIGN_CENTER)
{
}

/// destructor:
inline TextButton::~TextButton() = default;


#endif
