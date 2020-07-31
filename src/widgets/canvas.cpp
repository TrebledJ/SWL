/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
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
    Renderer const& m_renderer;
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
    m_named_items[id] = std::unique_ptr<WidgetItem>(item);
    return *this;
}

Canvas& Canvas::add_item(WidgetItem* item)
{
    m_unnamed_items.push_back(std::unique_ptr<WidgetItem>(item));
    return *this;
}

Canvas& Canvas::add_canvas(std::string const& id, Canvas* item)
{
    m_named_canvas[id] = std::unique_ptr<Canvas>(item);
    return *this;
}

Canvas& Canvas::add_canvas(Canvas* item)
{
    m_unnamed_canvas.push_back(std::unique_ptr<Canvas>(item));
    return *this;
}

void Canvas::foreach_child(std::function<void(WidgetItem*)> f)
{
    for (auto& pair : m_named_items) f(pair.second.get());
    for (auto& pair : m_named_canvas) f(pair.second.get());
    for (auto& child : m_unnamed_items) f(child.get());
    for (auto& child : m_unnamed_canvas) f(child.get());
}

/// accessors:
void Canvas::foreach_child(std::function<void(WidgetItem*)> f) const
{
    for (auto const& pair : m_named_items) f(pair.second.get());
    for (auto const& pair : m_named_canvas) f(pair.second.get());
    for (auto const& child : m_unnamed_items) f(child.get());
    for (auto const& child : m_unnamed_canvas) f(child.get());
}

/// GUI functions:
bool Canvas::handle_mouse_event(MouseEvent const& event)
{
    if (!RectItem::handle_mouse_event(event))
        return false;

    //  since canvas deals with offset'ed items, apply an offset to the event
    for (auto& pair : m_named_items) pair.second->handle_mouse_event(event.offset(m_dimensions.x, m_dimensions.y));
    for (auto& pair : m_named_canvas) pair.second->handle_mouse_event(event.offset(m_dimensions.x, m_dimensions.y));
    for (auto& child : m_unnamed_items) child->handle_mouse_event(event.offset(m_dimensions.x, m_dimensions.y));
    for (auto& child : m_unnamed_canvas) child->handle_mouse_event(event.offset(m_dimensions.x, m_dimensions.y));
    
    return true;
}

bool Canvas::handle_wheel_event(WheelEvent const& event)
{
    if (!RectItem::handle_wheel_event(event))
        return false;

    for (auto& pair : m_named_items) pair.second->handle_wheel_event(event.offset(m_dimensions.x, m_dimensions.y));
    for (auto& pair : m_named_canvas) pair.second->handle_wheel_event(event.offset(m_dimensions.x, m_dimensions.y));
    for (auto& child : m_unnamed_items) child->handle_wheel_event(event.offset(m_dimensions.x, m_dimensions.y));
    for (auto& child : m_unnamed_canvas) child->handle_wheel_event(event.offset(m_dimensions.x, m_dimensions.y));

    return true;
}

void Canvas::update(Renderer const& renderer)
{
    for (auto& pair : m_named_canvas)
        if (pair.second->is_visible())
            pair.second->update(renderer);
    
    for (auto& child : m_unnamed_canvas)
        if (child->is_visible())
            child->update(renderer);
    
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
    for (auto const& pair : m_named_items) pair.second->render(renderer);
    for (auto const& pair : m_named_canvas) pair.second->render(renderer);
    for (auto const& child : m_unnamed_items) child->render(renderer);
    for (auto const& child : m_unnamed_canvas) child->render(renderer);
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
    RectItem::swap(canvas);
    std::swap(m_texture, canvas.m_texture);
    std::swap(m_custom_redraw, canvas.m_custom_redraw);
    std::swap(m_redraw, canvas.m_redraw);
    std::swap(m_named_items, canvas.m_named_items);
    std::swap(m_named_canvas, canvas.m_named_canvas);
    std::swap(m_unnamed_items, canvas.m_unnamed_items);
    std::swap(m_unnamed_canvas, canvas.m_unnamed_canvas);
}
