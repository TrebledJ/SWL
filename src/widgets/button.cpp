/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#include "button.hpp"

#include "themes.hpp"
#include "utility.hpp"

#include "sdl_inc.hpp"


/// GUI functions:
bool Button::handle_mouse_event(MouseEvent const& event)
{
    //  filter events by relying on parent function
    if (!WidgetItem::handle_mouse_event(event))
        return false;
    
    switch (event.type)
    {
    case MouseEvent::UP:
        if (m_clicked) m_clicked(event);
        
        switch (event.button)
        {
        case SDL_BUTTON_LEFT:
            if (m_left_clicked) m_left_clicked(event);
            break;
        case SDL_BUTTON_RIGHT:
            if (m_right_clicked) m_right_clicked(event);
            break;
        case SDL_BUTTON_MIDDLE:
        default:
            break;
        }
        break;
        
    case MouseEvent::DOWN:
        if (m_pressed) m_pressed(event);
        break;
        
    case MouseEvent::MOTION:
    {
        auto state = event.button;
        auto lpressed = state & SDL_BUTTON_LMASK;
        auto rpressed = state & SDL_BUTTON_RMASK;
        if (!lpressed && !rpressed)
        {
            if (m_hovered) m_hovered(event);
        }
        
        break;
    }
    }
    return true;
}

/// convenience functions:
void Button::swap(Button& button) noexcept
{
    WidgetItem::swap(button);
    swap_members(button);
}

void Button::swap_members(Button& button) noexcept
{
    using std::swap;
    swap(m_clicked, button.m_clicked);
    swap(m_left_clicked, button.m_left_clicked);
    swap(m_right_clicked, button.m_right_clicked);
    swap(m_pressed, button.m_pressed);
    swap(m_hovered, button.m_hovered);
    swap(m_enabled, button.m_enabled);
}
