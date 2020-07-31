/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#include "textitem.hpp"


/// GUI functions:
bool TextItem::render(Renderer const& renderer) const
{
    if (!WidgetItem::render(renderer))
        return false;
    
    draw_text(renderer, m_dimensions, m_font.lock(), m_text, m_alignment);
    return true;
}

/// convenience functions:
void TextItem::swap(TextItem& item) noexcept
{
    WidgetItem::swap(item);
    swap_members(item);
}

void TextItem::swap_members(TextItem& item) noexcept
{
    using std::swap;
    swap(m_font, item.m_font);
    swap(m_text, item.m_text);
    swap(m_alignment, item.m_alignment);
}
