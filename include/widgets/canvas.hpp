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

#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "rectitem.hpp"

#include "themes.hpp"
#include "types.hpp"
#include "utility.hpp"

#include "sdl_inc.hpp"

#include <list>
#include <map>
#include <string>
#include <utility>


/**
 * @brief   Manages children and caches drawn stuff onto a texture. Renders
 *          child items and canvases RELATIVE to the parent canvas.
 *
 * @note    IMPORTANT: Remember to call make() to initialise the canvas.
 *
 * @note    The default redraw does (1) clear, (2) render children.
 *          A custom redraw replaces step 2. Note that the renderer passed
 *          will target the canvas, such that items are drawn relatively.
 */
class Canvas : public RectItem
{
public:
    using RedrawFunc = std::function<void(Renderer const&)>;

public:
    /// constructors:
    /**
     * These constructors will NOT initialise a texture.
     * I.e. the canvas will use a null texture.
     */
    Canvas(int width, int height);
    Canvas(SDL_Rect const&);
    
    /**
     * These constructors WILL initialise a texture, using the renderer passed.
     */
    Canvas(int width, int height, Renderer const&);
    Canvas(SDL_Rect const&, Renderer const&);
    
    Canvas(Canvas const&) = delete;
    Canvas(Canvas&&) noexcept;
    
    /// destructors:
    virtual ~Canvas();
    
    /// assignment:
    Canvas& operator= (Canvas const&) = delete;
    Canvas& operator= (Canvas&&) noexcept;
    
    /// modifiers:
    Canvas& custom_redraw(RedrawFunc);
    
    /**
     * @brief   Set the texture's blend mode
     * @pre     make() should have been called
     */
    Canvas& blend(SDL_BlendMode);
    
    /**
     * @brief   Redraws the texture the next time update() is called
     */
    Canvas& redraw();
    
    /**
     * @brief   Adds an item to the canvas. The item will be fully managed by
     *          the canvas (i.e. it will be deleted when the Canvas is destroyed).
     *
     *          Adding items through the second overload will leave it nameless and
     *          not possible to lookup using find().
     */
    Canvas& add_item(std::string const& id, WidgetItem*);
    Canvas& add_item(WidgetItem*);
    
    /**
     * @brief   Adds a child canvas. The child canvas will be managed by this class.
     *
     *          Adding items through the second overload will leave it nameless and
     *          not possible to lookup using find().
     */
    Canvas& add_canvas(std::string const& id, Canvas*);
    Canvas& add_canvas(Canvas*);
    
    void foreach_child(std::function<void(WidgetItem*)>);
    
    /// accessors:
    /**
     * @brief   Finds a child item with the given id and casts it to the given type.
     */
    template<class T = WidgetItem>
    T* child(std::string const& id) const;
    
    void foreach_child(std::function<void(WidgetItem*)>) const;
    
    /// GUI functions:
    virtual bool handle_mouse_event(MouseEvent const&) override;
    virtual bool handle_wheel_event(WheelEvent const&) override;
    
    /**
     * @brief   Update child canvases and perform the actual redraw
     *          Invisible child canvases are not updated
     */
    virtual void update(Renderer const&);
    
    /**
     * @brief   The sole responsibility of this override is to render the cached texture.
     *          Use redraw() and update() to update the texture.
     * @pre     make() the canvas first
     */
    virtual bool render(Renderer const&) const override;
    void render_children(Renderer const&) const;
    
    /// convenience functions:
    void swap(Canvas&) noexcept;
    
private:
    Texture m_texture;
    RedrawFunc m_custom_redraw;
    bool m_redraw;   //  whether canvas will redraw next render or not
    
    //  child widgets stored here will be rendered relative to the Canvas
    std::map<std::string, std::unique_ptr<WidgetItem>> m_named_items;
    std::map<std::string, std::unique_ptr<Canvas>> m_named_canvas;
    std::list<std::unique_ptr<WidgetItem>> m_unnamed_items;
    std::list<std::unique_ptr<Canvas>> m_unnamed_canvas;
    
private:
    /// helper functions:
    /**
     * @brief   Clears the canvas with the background color
     * @pre     make() the canvas first
     */
    Canvas& clear(Renderer const&);
    
    void perform_redraw(Renderer const&);
};


/// constructors:
inline Canvas::Canvas(int width, int height) : Canvas({0, 0, width, height}) {}
inline Canvas::Canvas(SDL_Rect const& dimensions)
    : RectItem(dimensions)
    , m_redraw{true}
{
}
inline Canvas::Canvas(int width, int height, Renderer const& renderer) : Canvas({0, 0, width, height}, renderer) {}
inline Canvas::Canvas(SDL_Rect const& dimensions, Renderer const& renderer)
    : RectItem(dimensions)
    , m_redraw{true}
    , m_texture{make_texture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dimensions.w, dimensions.h)}
{
}
inline Canvas::Canvas(Canvas&& other) noexcept
{
    swap(other);
    other.m_redraw = false;
}

/// destructors:
inline Canvas::~Canvas() = default;

/// assignment:
inline Canvas& Canvas::operator= (Canvas&& other) noexcept
{
    swap(other);
    other.m_redraw = false;
    return *this;
}

/// accessors:
template<class T>
T* Canvas::child(std::string const& id) const
{
    auto item_it = m_named_items.find(id);
    if (item_it != m_named_items.end())
        return dynamic_cast<T*>(item_it->second.get());
    
    auto canvas_it = m_named_canvas.find(id);
    if (canvas_it != m_named_canvas.end())
        return dynamic_cast<T*>(canvas_it->second.get());
    
    return nullptr;
}


#endif /* canvas_hpp */
