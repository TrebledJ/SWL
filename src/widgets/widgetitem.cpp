/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#include "widgetitem.hpp"
#include "types.hpp"
#include "utility.hpp"


/// convenience functions:
void WidgetItem::swap(WidgetItem& item) noexcept
{
    using std::swap;
    swap(m_dimensions, item.m_dimensions);
    swap(m_visible, item.m_visible);
}
