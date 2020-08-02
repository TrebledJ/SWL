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

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "rectitem.hpp"
#include "themes.hpp"
#include "types.hpp"

#include "sdl_inc.hpp"

#include <functional>


/**
 * @brief   A button widget, able to interact mouse events with clicks and hovers
 *
 * @note    Handler Events: SDL_MOUSEBUTTONUP, SDL_MOUSEBUTTON_DOWN, SDL_MOUSEBUTTONMOTION
 */
class Button : public RectItem
{
public:
    using MouseEventCallback = std::function<void(MouseEvent const&)>;
    
public:
    /// constructors:
    Button() noexcept;
    Button(SDL_Rect const&) noexcept;
    Button(Button const&);
    Button(Button&&) noexcept;
    
    /// destructor:
    virtual ~Button();
    
    /// assignment:
    Button& operator= (Button const&);
    Button& operator= (Button&&) noexcept;
    
    /// modifiers:
    Button& on_clicked(MouseEventCallback);
    Button& on_left_clicked(MouseEventCallback);
    Button& on_right_clicked(MouseEventCallback);
    Button& on_pressed(MouseEventCallback);
    Button& on_hovered(MouseEventCallback);

    /// GUI functions:
    virtual bool handle_mouse_event(MouseEvent const&) override;
    
    /// convenience functions:
    void swap(Button&) noexcept;
    
protected:
    MouseEventCallback m_clicked;
    MouseEventCallback m_left_clicked;
    MouseEventCallback m_right_clicked;
    MouseEventCallback m_pressed;
    MouseEventCallback m_hovered;
    
protected:
    void swap_members(Button&) noexcept;
};


/// constructors:
inline Button::Button() noexcept : Button(SDL_Rect{0}) {};
inline Button::Button(SDL_Rect const& dimensions) noexcept : RectItem(dimensions) {}
inline Button::Button(Button const&) = default;
inline Button::Button(Button&& button) noexcept : RectItem{std::move(button)}
{
    //  providing an implementation because CI cmake no likey =default
    swap_members(button);
}

/// destructor:
inline Button::~Button() = default;

/// assignment:
inline Button& Button::operator= (Button const&) = default;
inline Button& Button::operator= (Button&& button) noexcept
{
    swap(button);
    return *this;
}

/// modifiers:
inline Button& Button::on_clicked(MouseEventCallback f) { m_clicked = f; return *this; }
inline Button& Button::on_left_clicked(MouseEventCallback f) { m_left_clicked = f; return *this; }
inline Button& Button::on_right_clicked(MouseEventCallback f) { m_right_clicked = f; return *this; }
inline Button& Button::on_pressed(MouseEventCallback f) { m_pressed = f; return *this; }
inline Button& Button::on_hovered(MouseEventCallback f) { m_hovered = f; return *this; }


#endif
