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

#include "widgets/image.hpp"


/// GUI functions:
void ImageItem::render(Renderer const& renderer) const
{
    SDL_RenderCopy(renderer.get(), m_texture.get(), nullptr, &m_dimensions);
}

/// convenience functions:
void ImageItem::swap(ImageItem& other) noexcept
{
    std::swap(m_texture, other.m_texture);
}
