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

#include "canvas.hpp"
#include <iostream>
#include <cassert>
#include <functional>


/*-- class TargetWrapper --*/
/**
 * @brief   A wrapper class for applying a temporary target to a renderer.
 *
 * @note    The target is applied on object construction,
 *          and the preceding target is restored on object destruction.
 * @note    Usage:
 *              {
 *                  auto var = TargetWrapper{renderer, texture};
 *                  draw_rect(...);
 *                  draw_text(...);
 *                  if (condition)
 *                      return; //  original target restored
 *                  button.render(renderer);
 *              }   //  original target restored
 *
 *          OR
 *
 *              render(TargetWrapper{renderer, temp_texture});
 *
 */
class TargetWrapper
{
    Renderer const&  m_renderer;
    SDL_Texture* m_prev_target;
    bool m_target_ok;  //  TODO: change to C++17 std::optional
    
public:
    /// constructors:
    TargetWrapper(Renderer const& renderer, Texture const& target);
    TargetWrapper(TargetWrapper const&) = delete;
    TargetWrapper(TargetWrapper&& other);
    
    /// destructor:
    ~TargetWrapper();
    
    /// assignment:
    TargetWrapper& operator=(TargetWrapper const&) = delete;
    TargetWrapper& operator=(TargetWrapper&&) = delete;
    
    /// convenience functions:
    operator Renderer const&() const;
};

/// constructors:
TargetWrapper::TargetWrapper(Renderer const& renderer, Texture const& target)
    : m_renderer{renderer}
    , m_prev_target{SDL_GetRenderTarget(renderer.get())}
    , m_target_ok{true}
{
    SDL_SetRenderTarget(renderer.get(), target.get());
}
TargetWrapper::TargetWrapper(TargetWrapper&& other)
    : m_renderer{other.m_renderer}
    , m_prev_target{other.m_prev_target}
    , m_target_ok{true}
{
    other.m_target_ok = false;    //  set false to prevent "premature unwrapping"
}

/// destructor:
TargetWrapper::~TargetWrapper()
{
    if (m_target_ok)
        SDL_SetRenderTarget(m_renderer.get(), m_prev_target);
}

/// convenience functions:
TargetWrapper::operator Renderer const&() const
{
    return m_renderer;
}

/*-- class Canvas --*/

/// modifiers:
Canvas& Canvas::custom_redraw(RedrawFunc func)
{
    m_custom_redraw = func;
    return *this;
}

Canvas& Canvas::blend(SDL_BlendMode mode)
{
    assert(bool(m_texture));
    SDL_SetTextureBlendMode(m_texture.get(), mode);
    return *this;
}

Canvas& Canvas::redraw()
{
    m_redraw = true;
    return *this;
}

Canvas& Canvas::add_item(std::string const& id, WidgetItem* item)
{
    if (id.empty()) add_item(item);
    else m_items[id] = std::unique_ptr<WidgetItem>(item);
    return *this;
}

Canvas& Canvas::add_item(WidgetItem* item)
{
    m_items[std::to_string(m_counter++)] = std::unique_ptr<WidgetItem>(item);
    return *this;
}

Canvas& Canvas::add_canvas(std::string const& id, Canvas* item)
{
    if (id.empty()) add_canvas(item);
    else m_canvases[id] = std::unique_ptr<Canvas>(item);
    return *this;
}

Canvas& Canvas::add_canvas(Canvas* item)
{
    m_canvases[std::to_string(m_counter++)] = std::unique_ptr<Canvas>(item);
    return *this;
}

void Canvas::remove(WidgetItem* item)
{
    for (auto it = m_items.rbegin(); it != m_items.rend(); ++it)
        if (it->second.get() == item)
            m_items.erase(it.base());
    for (auto it = m_canvases.rbegin(); it != m_canvases.rend(); ++it)
        if (it->second.get() == item)
            m_canvases.erase(it.base());
}

void Canvas::remove(std::string const& id)
{
    const auto it = m_items.find(id);
    if (it != m_items.end())
        m_items.erase(it);
    
    const auto it2 = m_canvases.find(id);
    if (it2 != m_canvases.end())
        m_canvases.erase(it2);
}

void Canvas::foreach_child(std::function<void(WidgetItem*)> f)
{
    for (auto& pair : m_items) f(pair.second.get());
    for (auto& pair : m_canvases) f(pair.second.get());
}

/// accessors:
void Canvas::foreach_child(std::function<void(WidgetItem*)> f) const
{
    for (auto const& pair : m_items) f(pair.second.get());
    for (auto const& pair : m_canvases) f(pair.second.get());
}

/// GUI functions:
bool Canvas::handle_mouse_event(MouseEvent const& event)
{
    if (!Super::handle_mouse_event(event))
        return false;

    //  since canvas deals with offset'ed items, apply an offset to the event
    for (auto& pair : m_items) pair.second->handle_mouse_event(event.offset(m_dimensions.x, m_dimensions.y));
    for (auto& pair : m_canvases) pair.second->handle_mouse_event(event.offset(m_dimensions.x, m_dimensions.y));
    
    return true;
}

bool Canvas::handle_wheel_event(WheelEvent const& event)
{
    if (!Super::handle_wheel_event(event))
        return false;

    for (auto& pair : m_items) pair.second->handle_wheel_event(event.offset(m_dimensions.x, m_dimensions.y));
    for (auto& pair : m_canvases) pair.second->handle_wheel_event(event.offset(m_dimensions.x, m_dimensions.y));

    return true;
}

void Canvas::update(Renderer const& renderer)
{
    for (auto& pair : m_canvases)
        if (pair.second->is_visible())
            pair.second->update(renderer);
    
    if (m_redraw)
    {
        m_redraw = false;
        perform_redraw(m_texture ? TargetWrapper{renderer, m_texture} : renderer);
    }
}

bool Canvas::render(Renderer const& renderer) const
{
    //  not using RectItem::render because we want to use the background only during redraw
    if (!WidgetItem::render(renderer))
        return false;
    
    if (m_texture)
        SDL_RenderCopy(renderer.get(), m_texture.get(), nullptr, &m_dimensions);
    return true;
}

void Canvas::render_children(Renderer const& renderer) const
{
    for (auto const& pair : m_items) pair.second->render(renderer);
    for (auto const& pair : m_canvases) pair.second->render(renderer);
}

/// helper functions:
Canvas& Canvas::clear(Renderer const& renderer)
{
    set_render_color(renderer, m_background_color);
    SDL_RenderClear(renderer.get());
    return *this;
}

void Canvas::perform_redraw(Renderer const& renderer)
{
    this->clear(renderer);
    if (m_custom_redraw)
        m_custom_redraw(renderer);
    else
        render_children(renderer);
}

void Canvas::swap(Canvas& canvas) noexcept
{
    Super::swap(canvas);
    std::swap(m_texture, canvas.m_texture);
    std::swap(m_custom_redraw, canvas.m_custom_redraw);
    std::swap(m_redraw, canvas.m_redraw);
    std::swap(m_items, canvas.m_items);
    std::swap(m_canvases, canvas.m_canvases);
}
