/*
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

#include "utility.hpp"


void draw_text(Renderer const& renderer, int x, int y, TTFont const& font, SDL_Color const& color, std::string const& text)
{
    if (!font)
        return;
    
    auto surface = make_surface(font, text, color);
    auto texture = make_texture_from_surface(renderer, surface);
    
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &rect);
}

void draw_text(Renderer const& renderer, SDL_Rect const& bounds, SharedFont const& font, std::string const& text,
               Alignment align)
{
    if (!font)
        return;
    
    auto x = bounds.x;
    auto y = bounds.y;
    auto width = FC_GetWidth(font.get(), text.data());
    auto height = FC_GetHeight(font.get(), text.data());
    
    if (align & ALIGN_LEFT)
    {
    }
    else if (align & ALIGN_HCENTER)
        x += (bounds.w - width) / 2;
    else if (align & ALIGN_RIGHT)
        x += bounds.w - width;
    
    if (align & ALIGN_TOP)
    {
    }
    else if (align & ALIGN_VCENTER)
        y += (bounds.h - height) / 2;
    else if (align & ALIGN_BOTTOM)
        y += bounds.h - height;
    
    draw_simple_text(renderer, x, y, font, text);
}

void draw_centered_text(Renderer const& renderer, SDL_Rect const& bounds, TTFont const& font, SDL_Color const& color, std::string const& text)
{
    if (!font)
        return;
    
    auto surface = make_surface(font, text, color);
    auto texture = make_texture_from_surface(renderer, surface);
    
    SDL_Rect rect = {bounds.x + (bounds.w - surface->w)/2, bounds.y + (bounds.h - surface->h)/2, surface->w, surface->h};
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &rect);
}

namespace Util
{
    void replace(std::string& str, std::string const& text, std::string const& repl)
    {
        auto pos = str.find(text);
        if (pos != std::string::npos)
            str.replace(pos, text.size(), repl);
    }
}
