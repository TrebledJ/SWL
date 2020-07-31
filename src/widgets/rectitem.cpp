/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#include "rectitem.hpp"
#include "utility.hpp"


bool RectItem::render(Renderer const& renderer) const
{
    if (!WidgetItem::render(renderer))
        return false;
    
    if (m_enabled)
        draw_filled_rect(renderer, m_dimensions, m_background_color);
    else
        draw_filled_rect(renderer, m_dimensions, Colors::darken(m_background_color, 0.5));
    return true;
}
