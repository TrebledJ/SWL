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

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "types.hpp"

#include "sdl_image_inc.hpp"
#include "sdl_mixer_inc.hpp"
#include "sdl_ttf_inc.hpp"
#include "sdl_inc.hpp"

#include <string>


enum Alignment
{
    ALIGN_LEFT      = (1 << 0),
    ALIGN_HCENTER   = (1 << 1),
    ALIGN_RIGHT     = (1 << 2),
    ALIGN_TOP       = (1 << 3),
    ALIGN_VCENTER   = (1 << 4),
    ALIGN_BOTTOM    = (1 << 5),
    ALIGN_TOP_LEFT      = ALIGN_LEFT | ALIGN_TOP,
    ALIGN_TOP_CENTER    = ALIGN_HCENTER | ALIGN_TOP,
    ALIGN_TOP_RIGHT     = ALIGN_RIGHT | ALIGN_TOP,
    ALIGN_CENTER_LEFT   = ALIGN_LEFT | ALIGN_VCENTER,
    ALIGN_CENTER        = ALIGN_HCENTER | ALIGN_VCENTER,
    ALIGN_CENTER_RIGHT  = ALIGN_RIGHT | ALIGN_VCENTER,
    ALIGN_BOTTOM_LEFT   = ALIGN_LEFT | ALIGN_BOTTOM,
    ALIGN_BOTTOM_CENTER = ALIGN_HCENTER | ALIGN_BOTTOM,
    ALIGN_BOTTOM_RIGHT  = ALIGN_RIGHT | ALIGN_BOTTOM,
};

//  wrapper initialisers
Surface make_surface(SDL_Surface* surface);
Texture make_texture(SDL_Texture* texture);
Renderer make_renderer(SDL_Renderer* renderer);
Window make_window(SDL_Window* window);
TTFont make_font(TTF_Font* font);

//  SDL-like initialisers
Surface make_surface(TTFont const& font, std::string const& text, SDL_Color const& color);
Texture make_texture(Renderer const& renderer, Uint32 format, int access, int w, int h);
Texture make_texture_from_surface(Renderer const& renderer, Surface const& surface);
Renderer make_renderer(Window const& window, int index, Uint32 flags);
Window make_window(std::string const& title, int x, int y, int width, int height, Uint32 flags);
TTFont make_font(std::string const& filename, unsigned font_size);
SharedFont make_shared_font(Renderer const& renderer, std::string const& filename, Uint32 point_size,
                            SDL_Color const& color, int style = TTF_STYLE_NORMAL);
SharedMusic make_shared_music(std::string const& source);

//  render utility functions
void set_render_color(Renderer const& renderer, SDL_Color const& color);
void reset_target(Renderer const& renderer);
void render_surface(Renderer const& renderer, Surface const& surface, int x, int y);
void render_surface(Renderer const& renderer, Surface const& surface, int x, int y, int w, int h);
void render_texture(Renderer const& renderer, Texture const& texture, SDL_Rect const& bounds);
void render_texture(Renderer const& renderer, Texture const& texture, int x, int y);
void render_texture(Renderer const& renderer, Texture const& texture, int x, int y, int w, int h);

//  text utility functions
void draw_simple_text(Renderer const& renderer, int x, int y, SharedFont const& font, std::string const& text);
void draw_text(Renderer const&, int x, int y, TTFont const&, SDL_Color const&, std::string const& text);
void draw_text(Renderer const&, SDL_Rect const& bounds, SharedFont const&, std::string const& text, Alignment = ALIGN_TOP_LEFT);
void draw_centered_text(Renderer const&, SDL_Rect const& bounds, TTFont const&, SDL_Color const&, std::string const& text);
void draw_centered_text(Renderer const& renderer, SDL_Rect const& bounds, SharedFont const& font, std::string const& text);
void draw_rect(Renderer const& renderer, SDL_Rect const& rect, SDL_Color const& color);
void draw_filled_rect(Renderer const& renderer, SDL_Rect const& rect, SDL_Color const& color);

/// misc. utility functions:
namespace Util
{
    /// @brief  Filters SDL_MOUSEBUTTONUP, SDL_MOUSEBUTTONDOWN, SDL_MOUSEMOTION events
    bool is_mouse_event(SDL_Event const& event);

    /// @brief  Filters SDL_MOUSEMOTION events
    bool is_motion_event(SDL_Event const& event);

    /// @brief   Filters SDL_MOUSEWHEEL events
    bool is_wheel_event(SDL_Event const& event);

    /// @pre    `event` is a mouse event
    MouseEvent make_mouse_event(SDL_Event const& event);
    /// @pre    `event` is a wheel event
    WheelEvent make_wheel_event(SDL_Event const& event);
    /// @pre    `event` is a keyboard event
    KeyEvent make_key_event(SDL_Event const& event);
    
    void replace(std::string& str, std::string const& text, std::string const& repl);
    
    /**
     * @brief   Tests for true. Throws exception if `value` evaluates to false.
     * @note    Message Tokens: see assert_equals()
     */
    template<class T>
    void assert_true(T const& value, std::string msg = "An error occurred!");
    
    /**
     * @brief   Tests for equality. Throws exception if unequal.
     * @note    Message Tokens: ${sdl_error}, ${ttf_error}, ${value}
     */
    template<class T>
    void assert_equals(T const& value, T const& test, std::string msg = "An error occurred!");
}



//
//  Implementation
//

//  wrapper initialisers
static void do_nothing(TTF_Font*) {}
inline Surface make_surface(SDL_Surface* surface) { return Surface(surface, SDL_FreeSurface); }
inline Texture make_texture(SDL_Texture* texture) { return Texture(texture, SDL_DestroyTexture); }
inline Renderer make_renderer(SDL_Renderer* renderer) { return Renderer(renderer, SDL_DestroyRenderer); }
inline Window make_window(SDL_Window* window) { return Window(window, SDL_DestroyWindow); }
inline TTFont make_font(TTF_Font* font) { return TTFont(font, do_nothing); }

//  SDL-like initialisers
inline Surface make_surface(TTFont const& font, std::string const& text, SDL_Color const& color)
{
    return make_surface(TTF_RenderText_Solid(font.get(), text.data(), color));
}

inline Texture make_texture(Renderer const& renderer, Uint32 format, int access, int w, int h)
{
    return make_texture(SDL_CreateTexture(renderer.get(), format, access, w, h));
}

inline Texture make_texture_from_surface(Renderer const& renderer, Surface const& surface)
{
    return make_texture(SDL_CreateTextureFromSurface(renderer.get(), surface.get()));
}

inline Renderer make_renderer(Window const& window, int index, Uint32 flags)
{
    return make_renderer(SDL_CreateRenderer(window.get(), index, flags));
}

inline Window make_window(std::string const& title, int x, int y, int width, int height, Uint32 flags)
{
    return make_window(SDL_CreateWindow(title.data(), x, y, width, height, flags));
}

inline TTFont make_font(std::string const& filename, unsigned font_size)
{
    return make_font(TTF_OpenFont(filename.data(), font_size));
}

inline SharedFont make_shared_font(Renderer const& renderer, std::string const& filename, Uint32 point_size,
                                   SDL_Color const& color, int style)
{
    auto font = std::shared_ptr<FC_Font>(FC_CreateFont(), FC_FreeFont);
    FC_LoadFont(font.get(), renderer.get(), filename.data(), point_size, color, style);
    return font;
}

inline SharedMusic make_shared_music(std::string const& source)
{
    return SharedMusic(Mix_LoadMUS(source.data()), Mix_FreeMusic);
}

//  render utility functions
inline void set_render_color(Renderer const& renderer, SDL_Color const& color)
{
    SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, color.a);
}

inline void reset_target(Renderer const& renderer)
{
    SDL_SetRenderTarget(renderer.get(), nullptr);
}

//inline void render_cropped_surface(Renderer const& renderer, Surface const& surface, int x, int y)
//{
//    render_texture(renderer, make_texture_from_surface(renderer, surface), x, y);
//}

inline void render_surface(Renderer const& renderer, Surface const& surface, int x, int y)
{
    render_texture(renderer, make_texture_from_surface(renderer, surface), x, y);
}

inline void render_surface(Renderer const& renderer, Surface const& surface, int x, int y, int w, int h)
{
    render_texture(renderer, make_texture_from_surface(renderer, surface), x, y, w, h);
}

inline void render_texture(Renderer const& renderer, Texture const& texture, SDL_Rect const& dest)
{
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &dest);
}

inline void render_texture(Renderer const& renderer, Texture const& texture, int x, int y)
{
    int w, h;
    SDL_QueryTexture(texture.get(), nullptr, nullptr, &w, &h);
    render_texture(renderer, texture, {x, y, w, h});
}

inline void render_texture(Renderer const& renderer, Texture const& texture, int x, int y, int w, int h)
{
    render_texture(renderer, texture, {x, y, w, h});
}

//  text utility functions
inline void draw_simple_text(Renderer const& renderer, int x, int y, SharedFont const& font, std::string const& text)
{
    if (font)
        FC_Draw(font.get(), renderer.get(), x, y, text.data());
}

inline void draw_centered_text(Renderer const& renderer, SDL_Rect const& bounds, SharedFont const& font, std::string const& text)
{
    draw_text(renderer, bounds, font, text, ALIGN_CENTER);
}

inline void draw_rect(Renderer const& renderer, SDL_Rect const& rect, SDL_Color const& color)
{
    set_render_color(renderer, color);
    SDL_RenderDrawRect(renderer.get(), &rect);
}

inline void draw_filled_rect(Renderer const& renderer, SDL_Rect const& rect, SDL_Color const& color)
{
    set_render_color(renderer, color);
    SDL_RenderFillRect(renderer.get(), &rect);
}

inline void draw_surface(Renderer const& renderer, Surface const& surface, SDL_Rect const& dest)
{
    Texture texture = make_texture_from_surface(renderer, surface);
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &dest);
}

/// misc. utility functions:
namespace Util
{
    /// @brief  Filters SDL_MOUSEBUTTONUP, SDL_MOUSEBUTTONDOWN, SDL_MOUSEMOTION events
    inline bool is_mouse_event(SDL_Event const& event)
    {
        return event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN
            || event.type == SDL_MOUSEMOTION;
    }

    /// @brief  Filters SDL_MOUSEMOTION events
    inline bool is_motion_event(SDL_Event const& event)
    {
        return event.type == SDL_MOUSEMOTION;
    }

    /// @brief   Filters SDL_MOUSEWHEEL events
    inline bool is_wheel_event(SDL_Event const& event)
    {
        return event.type == SDL_MOUSEWHEEL;
    }

    /// @pre    `event` is a mouse event
    inline MouseEvent make_mouse_event(SDL_Event const& event)
    {
        return is_motion_event(event) ? MouseEvent(event.motion) : MouseEvent(event.button);
    }
    
    /// @pre    `event` is a wheel event
    inline WheelEvent make_wheel_event(SDL_Event const& event)
    {
        return WheelEvent(event.wheel);
    }
    
    /// @pre    `event` is a keyboard event
    inline KeyEvent make_key_event(SDL_Event const& event)
    {
        //  implementation may change in the future
        return event.key;
    }
    
    template<class T>
    void assert_true(T const& value, std::string msg)
    {
        assert_equals(bool(value), true, std::move(msg));
    }

    template<class T>
    void assert_equals(T const& value, T const& test, std::string msg)
    {
        if (value != test)
        {
            if (!msg.empty())
            {
                Util::replace(msg, "${sdl_error}", SDL_GetError());
                Util::replace(msg, "${img_error}", IMG_GetError());
                Util::replace(msg, "${ttf_error}", TTF_GetError());
                Util::replace(msg, "${mix_error}", Mix_GetError());
                Util::replace(msg, "${value}", std::to_string(value));
            }
            throw std::runtime_error{msg};
        }
    }
}


#endif
