/*
 *     Copyright (C) 2020 Johnathan Law
 *
 *     This file is part of SWL.
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

#ifndef INTERFACE_BUTTON_HPP
#define INTERFACE_BUTTON_HPP

#include "types.hpp"
#include <functional>


/**
 * @brief   An interface for mouse event handlers
 *
 *          The user can set handlers through the on_event functions.
 *          This interface allows you to override behaviour when
 *
 */
class ButtonInterface
{
public:
    using MouseEventCallback = std::function<void(MouseEvent const&)>;
    
public:
    /// destructor:
    virtual ~ButtonInterface();
    
    /// modifiers:
    void on_clicked(MouseEventCallback);
    void on_left_clicked(MouseEventCallback);
    void on_right_clicked(MouseEventCallback);
    void on_pressed(MouseEventCallback);
    void on_hovered(MouseEventCallback);
    
protected:
    virtual void clicked(MouseEvent const&) const;
    virtual void left_clicked(MouseEvent const&) const;
    virtual void right_clicked(MouseEvent const&) const;
    virtual void pressed(MouseEvent const&) const;
    virtual void hovered(MouseEvent const&) const;
    
    void handle(MouseEvent const& event) const;
    
private:
    MouseEventCallback m_clicked;
    MouseEventCallback m_left_clicked;
    MouseEventCallback m_right_clicked;
    MouseEventCallback m_pressed;
    MouseEventCallback m_hovered;

};


/// destructor:
inline ButtonInterface::~ButtonInterface() = default;

/// modifiers:
inline void ButtonInterface::on_clicked(MouseEventCallback f) { m_clicked = f; }
inline void ButtonInterface::on_left_clicked(MouseEventCallback f) { m_left_clicked = f; }
inline void ButtonInterface::on_right_clicked(MouseEventCallback f) { m_right_clicked = f; }
inline void ButtonInterface::on_pressed(MouseEventCallback f) { m_pressed = f; }
inline void ButtonInterface::on_hovered(MouseEventCallback f) { m_hovered = f; }

inline void ButtonInterface::clicked(MouseEvent const& event) const { if (m_clicked) m_clicked(event); }
inline void ButtonInterface::left_clicked(MouseEvent const& event) const { if (m_left_clicked) m_left_clicked(event); }
inline void ButtonInterface::right_clicked(MouseEvent const& event) const { if (m_right_clicked) m_right_clicked(event); }
inline void ButtonInterface::pressed(MouseEvent const& event) const { if (m_pressed) m_pressed(event); }
inline void ButtonInterface::hovered(MouseEvent const& event) const { if (m_hovered) m_hovered(event); }



#endif /* button_hpp */
