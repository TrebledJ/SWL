/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#include "textbutton.hpp"

#include "themes.hpp"
#include "utility.hpp"

#include "sdl_inc.hpp"


/// convenience functions:
void TextButton::swap(TextButton& button) noexcept
{
    Button::swap(button);           //  swaps Item + Button members
    TextItem::swap_members(button); //  swaps only TextItem members
}
