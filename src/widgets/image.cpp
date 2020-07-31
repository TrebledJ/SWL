/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#include "image.hpp"


/// GUI functions:
bool ImageItem::render(Renderer const& renderer) const
{
    if (!WidgetItem::render(renderer))
        return false;
    
    SDL_RenderCopy(renderer.get(), m_texture.get(), nullptr, &m_dimensions);
    return true;
}

/// convenience functions:
void ImageItem::swap(ImageItem& other) noexcept
{
    std::swap(m_texture, other.m_texture);
}
