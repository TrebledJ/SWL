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

#include "widgets/canvas.hpp"
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
using namespace std::placeholders;

#define findmove(from, to)  \
    {\
        auto it = from.find(id);\
        if (it != from.end())\
        {\
            /* insert item into target container */\
            to[it->first] = std::move(it->second);\
            \
            /* remove item from first container */\
            from.erase(it);\
            return;\
        }\
    }

#define moveall(from, to)   \
    for (auto it = from.begin(); it != from.end();)\
    {\
        to[it->first] = std::move(it->second);\
        it = from.erase(it);\
    }

#define iterate(container)  \
    for (auto it = container.begin(), next = it; it != container.end(); it = next)\
    {\
        ++next;\
        f(it->second.get());\
    }

#define findret(container) \
    {\
        auto it = container.find(id);\
        if (it != container.end())\
        return it->second.get();\
    }

/// modifiers:
Canvas& Canvas::on_redraw(RedrawFunc func)
{
    m_on_redraw = func;
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

ItemID Canvas::add_item(std::string const& name, WidgetItem* item)
{
    if (name.empty())
    {
        return add_item(item);
    }
    else
    {
        m_visible_items[++m_id_counter] = std::unique_ptr<WidgetItem>(item);
        m_name_to_id[name] = m_id_counter;
        item->id = m_id_counter;
        return m_id_counter;
    }
}

ItemID Canvas::add_item(WidgetItem* item)
{
    m_visible_items[++m_id_counter] = std::unique_ptr<WidgetItem>(item);
    item->id = m_id_counter;
    return m_id_counter;
}

ItemID Canvas::add_canvas(std::string const& name, Canvas* item)
{
    if (name.empty())
    {
        return add_canvas(item);
    }
    else
    {
        m_visible_canvases[++m_id_counter] = std::unique_ptr<Canvas>(item);
        m_name_to_id[name] = m_id_counter;
        item->id = m_id_counter;
        return m_id_counter;
    }
}

ItemID Canvas::add_canvas(Canvas* item)
{
    m_visible_canvases[++m_id_counter] = std::unique_ptr<Canvas>(item);
    item->id = m_id_counter;
    return m_id_counter;
}

//void Canvas::remove(WidgetItem* item)
//{
//    for (auto it = m_visible_items.begin(); it != m_visible_items.end(); ++it)
//        if (it->second.get() == item)
//            m_visible_items.erase(it);
//
//    for (auto it = m_invisible_items.begin(); it != m_invisible_items.end(); ++it)
//        if (it->second.get() == item)
//            m_invisible_items.erase(it);
//
//    for (auto it = m_visible_canvases.rbegin(); it != m_visible_canvases.rend(); ++it)
//        if (it->second.get() == item)
//            m_visible_canvases.erase(it.base());
//
//    for (auto it = m_invisible_canvases.rbegin(); it != m_invisible_canvases.rend(); ++it)
//        if (it->second.get() == item)
//            m_invisible_canvases.erase(it.base());
//}

//void Canvas::remove(ItemID id)
//{
//    auto item_it = m_visible_items.find(id);
//    if (item_it != m_visible_items.end())
//        m_visible_items.erase(item_it);
//
//    item_it = m_invisible_items.find(id);
//    if (item_it != m_invisible_items.end())
//        m_invisible_items.erase(item_it);
//
//    auto canvas_it = m_visible_canvases.find(id);
//    if (canvas_it != m_visible_canvases.end())
//        m_visible_canvases.erase(canvas_it);
//
//    auto canvas_it = m_invisible_canvases.find(id);
//    if (canvas_it != m_invisible_canvases.end())
//        m_invisible_canvases.erase(canvas_it);
//}

void Canvas::foreach_child(std::function<void(WidgetItem*)> f, ChildFlags flags)
{
    //  iterate over containers based on flags
    if (flags & VISIBLE)
    {
        if (flags & ITEMS) { iterate(m_visible_items) }
        else if (flags & CANVASES) { iterate(m_visible_canvases) }
        else
        {
            //  only VISIBLE was specified but neither ITEMS/CANVASES were... so apply `f` to all
            iterate(m_visible_items)
            iterate(m_visible_canvases)
        }
    }
    
    if (flags & INVISIBLE)
    {
        if (flags & ITEMS) { iterate(m_invisible_items) }
        else if (flags & CANVASES) { iterate(m_invisible_canvases) }
        else
        {
            iterate(m_invisible_items)
            iterate(m_invisible_canvases)
        }
    }
}
    
void Canvas::show(ItemID id)
{
    //  move item with given id from invisible to visible
    findmove(m_invisible_items, m_visible_items)
    findmove(m_invisible_canvases, m_visible_canvases)
}

void Canvas::hide(ItemID id)
{
    //  move item with given id from visible to invisible
    findmove(m_visible_items, m_invisible_items)
    findmove(m_visible_canvases, m_invisible_canvases)
}

void Canvas::show_children()
{
    moveall(m_invisible_items, m_visible_items)
    moveall(m_invisible_canvases, m_visible_canvases)
}

void Canvas::hide_children()
{
    moveall(m_visible_items, m_invisible_items)
    moveall(m_visible_canvases, m_invisible_canvases)
}

/// accessors:
WidgetItem* Canvas::child(std::string const& name) const
{
    auto it = m_name_to_id.find(name);
    return it != m_name_to_id.end() ? child(it->second) : nullptr;
}

WidgetItem* Canvas::child(ItemID id) const
{
    if (id != 0)
    {
        //  look for the child in each container; return if found
        findret(m_visible_items)
        findret(m_invisible_items)
        findret(m_visible_canvases)
        findret(m_invisible_canvases)
    }
    
    return nullptr;
}

void Canvas::foreach_child(std::function<void(WidgetItem*)> f, ChildFlags flags) const
{
    //  iterate over containers based on flags
    if (flags & VISIBLE)
    {
        if (flags & ITEMS) { iterate(m_visible_items) }
        else if (flags & CANVASES) { iterate(m_visible_canvases) }
        else
        {
            //  only VISIBLE was specified but neither ITEMS/CANVASES were... so apply `f` to all
            iterate(m_visible_items)
            iterate(m_visible_canvases)
        }
    }
    
    if (flags & INVISIBLE)
    {
        if (flags & ITEMS) { iterate(m_invisible_items) }
        else if (flags & CANVASES) { iterate(m_invisible_canvases) }
        else
        {
            iterate(m_invisible_items)
            iterate(m_invisible_canvases)
        }
    }
}

/// GUI functions:
bool Canvas::handle_mouse_event(MouseEvent const& event)
{
    if (!Super::handle_mouse_event(event))
        return false;

    //  since canvas deals with offset'ed items, apply an offset to the event
    const auto offset_event = event.offset(m_dimensions.x, m_dimensions.y);
    foreach_child(std::bind(&WidgetItem::handle_mouse_event, _1, std::cref(offset_event)), VISIBLE);
    return true;
}

bool Canvas::handle_wheel_event(WheelEvent const& event)
{
    if (!Super::handle_wheel_event(event))
        return false;

    const auto offset_event = event.offset(m_dimensions.x, m_dimensions.y);
    foreach_child(std::bind(&WidgetItem::handle_wheel_event, _1, std::cref(offset_event)), VISIBLE);
    return true;
}

void Canvas::update(Renderer const& renderer)
{
    for (auto& pair : m_visible_canvases)
        pair.second->update(renderer);
    
    if (m_redraw)
    {
        m_redraw = false;
        perform_redraw(m_texture ? TargetWrapper{renderer, m_texture} : renderer);
    }
}

void Canvas::render(Renderer const& renderer) const
{
    if (m_texture)
        SDL_RenderCopy(renderer.get(), m_texture.get(), nullptr, &m_dimensions);
}

void Canvas::render_children(Renderer const& renderer) const
{
    foreach_child(std::bind(&WidgetItem::render, _1, std::cref(renderer)), VISIBLE);
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
    if (m_on_redraw)
        m_on_redraw(renderer);
    else
        render_children(renderer);  //  default to simply rendering the children
}

void Canvas::swap(Canvas& canvas) noexcept
{
    Super::swap(canvas);
    
    using std::swap;
    swap(m_texture, canvas.m_texture);
    swap(m_on_redraw, canvas.m_on_redraw);
    swap(m_redraw, canvas.m_redraw);
    swap(m_visible_items, canvas.m_visible_items);
    swap(m_invisible_items, canvas.m_invisible_items);
    swap(m_visible_canvases, canvas.m_visible_canvases);
    swap(m_invisible_canvases, canvas.m_invisible_canvases);
    swap(m_id_counter, canvas.m_id_counter);
}
