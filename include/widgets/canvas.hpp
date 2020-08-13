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
#include <cstdint>


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
    using Super = RectItem;
    
public:
    using RedrawFunc = std::function<void(Renderer const&)>;
    
    enum ChildFlags {
        VISIBLE = (1 << 0),
        INVISIBLE = (1 << 1),
        ITEMS = (1 << 2),
        CANVASES = (1 << 3),
        
        ALL = VISIBLE | INVISIBLE | ITEMS | CANVASES,
    };

public:
    /// constructors:
    /**
     * These constructors will NOT initialise a texture (i.e. the canvas will use a null texture).
     */
    Canvas(int width, int height);
    Canvas(SDL_Rect const&);
    
    /**
     * These constructors WILL initialise a texture, using the renderer passed.
     */
    Canvas(int width, int height, Renderer const&, Canvas* parent = nullptr, std::string const& id = "");
    Canvas(SDL_Rect const&, Renderer const&, Canvas* parent = nullptr, std::string const& id = "");
    
    Canvas(Canvas const&) = delete;
    Canvas(Canvas&&) = delete;
    
    /// destructors:
    virtual ~Canvas();
    
    /// assignment:
    Canvas& operator= (Canvas const&) = delete;
    Canvas& operator= (Canvas&&) = delete;
    
    /// modifiers:
    Canvas& on_redraw(RedrawFunc);
    
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
     *          Adding items through the second overload will give it an anonymous name.
     */
    ItemID add_item(std::string const& name, WidgetItem*);
    ItemID add_item(WidgetItem*);
    
    /**
     * @brief   Adds a child canvas. The child canvas will be managed by this class.
     *
     *          Adding items through the second overload will leave it nameless and
     *          not possible to lookup using find().
     */
    ItemID add_canvas(std::string const& name, Canvas*);
    ItemID add_canvas(Canvas*);
    
//    void remove(WidgetItem*);
//    void remove(ItemID);
    
    /**
     * @brief   Apply a function to each children.
     *          The function may call an operation that removes the item
     *          (e.g. [](WidgetItem* w) { w->hide(); })
     *
     * @param   flags Which children to apply the function on.
     *          (e.g. flags == VISIBLE | CANVASES will only apply the function
     *          on visible canvases)
     */
    void foreach_child(std::function<void(WidgetItem*)>, ChildFlags flags = ALL);
    
    /**
     * @brief   Shows/hides a child by item id.
     */
    void show(ItemID);
    void hide(ItemID);
    void show_children();
    void hide_children();
    
    /// accessors:
    /**
     * @brief   Finds a child by name or id.
     */
    WidgetItem* child(std::string const& name) const;
    WidgetItem* child(ItemID id) const;
    
    /**
     * @brief   This version is a const version of foreach_child. Refer to the other
     *          version for usage guidance.
     */
    void foreach_child(std::function<void(WidgetItem*)>, ChildFlags = ALL) const;
    
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
    virtual void render(Renderer const&) const override;
    void render_children(Renderer const&) const;
    
    /// convenience functions:
    void swap(Canvas&) noexcept;
    
private:
    Texture m_texture;
    RedrawFunc m_on_redraw;
    bool m_redraw;   //  whether canvas will redraw next render or not
    
    //  child widgets stored here will be rendered relative to the Canvas
    std::map<ItemID, std::unique_ptr<Canvas>> m_visible_canvases;
    std::map<ItemID, std::unique_ptr<Canvas>> m_invisible_canvases;
    
    std::map<ItemID, std::unique_ptr<WidgetItem>> m_visible_items;
    std::map<ItemID, std::unique_ptr<WidgetItem>> m_invisible_items;
    std::map<std::string, ItemID> m_name_to_id;
    
    ItemID m_id_counter = 0;    //  used for keeping track of ID and for generating names of unnamed children
    
private:
    /// helper functions:
    /**
     * @brief   Clears the canvas with the background color
     */
    Canvas& clear(Renderer const&);
    
    void perform_redraw(Renderer const&);
};


/// constructors:
inline Canvas::Canvas(int width, int height) : Canvas({0, 0, width, height}) {}
inline Canvas::Canvas(SDL_Rect const& dimensions)
    : Super(dimensions)
    , m_redraw{true}
{
}
inline Canvas::Canvas(int width, int height, Renderer const& renderer, Canvas* parent, std::string const& name) : Canvas({0, 0, width, height}, renderer, parent, name) {}
inline Canvas::Canvas(SDL_Rect const& dimensions, Renderer const& renderer, Canvas* parent, std::string const& name)
    : Super(dimensions)
    , m_redraw{true}
    , m_texture{make_texture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dimensions.w, dimensions.h)}
{
    if ((m_parent = parent))
        m_parent->add_canvas(name, this);
}

/// destructors:
inline Canvas::~Canvas() = default;


#endif /* canvas_hpp */
