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

#ifndef ITEM_HPP
#define ITEM_HPP

#include "types.hpp"
#include "sdl_inc.hpp"


class Canvas;

/**
 * @brief   A basic item, the backbone of all widgets
 *
 * Widgets can be hidden or disabled. Hidden widgets should neither render nor
 * respond to events. Disabled widgets should have a dull hint of opacity,
 * indicating an inactive state.
 */
class WidgetItem
{
public:
    /// constructors:
    WidgetItem(Canvas* parent = nullptr, std::string const& name = "") noexcept;
    WidgetItem(SDL_Rect const& dimensions, Canvas* parent = nullptr, std::string const& name = "") noexcept;
    WidgetItem(WidgetItem const&) = delete;
    WidgetItem(WidgetItem&&);
    
    /// destructor:
    virtual ~WidgetItem();
    
    /// assignment:
    WidgetItem& operator= (WidgetItem const&) = delete;
    WidgetItem& operator= (WidgetItem&&) = delete;
    
    /// modifiers:
    WidgetItem& pos(int x, int y);
    WidgetItem& size(int w, int h);
    WidgetItem& x(int x);
    WidgetItem& y(int y);
    WidgetItem& width(int width);
    WidgetItem& height(int height);
    void show();
    void hide();
    void enable();
    void disable();
    
    /// accessors:
    int x() const;
    int y() const;
    int width() const;
    int height() const;
    Point pos() const;
    Size size() const;
    SDL_Rect dimensions() const;
    
    /// @return true if the item is enabled, otherwise false
    virtual bool is_enabled() const;
    
    /// GUI functions:
    /**
     * @brief   Handles events.
     * @return  true if the event was handled, false otherwise.
     *          For mouse/wheel events, an event is "handled" if the item is visible.
     *          For key events, an event is "handled" if the item responds to the key.
     *
     * On a normal basis, these functions doesn't need to be overridden.
     * Override only when needed
     *  (to introduce different interactive behaviours, delegating to children, etc.).
     *
     * See Canvas for an example of overriding handle_mouse_event() and handle_wheel_event().
     */
    virtual bool handle_mouse_event(MouseEvent const&);
    virtual bool handle_wheel_event(WheelEvent const&);
    virtual bool handle_key_event(KeyEvent const&);
    
    /**
     * @brief   Renders an item. This should be implemented such that
     *          calling render() will immediately draw the item on the
     *          renderer.
     *
     * See RectItem and Canvas for an example of implementing render().
     */
    virtual void render(Renderer const&) const = 0;
    
    /// convenience functions:
    void swap(WidgetItem&) noexcept;
    
    /**
     * @brief   Checks if the point x, y is inside the dimensions
     *          (treated as global relative to window)
     */
    bool is_point_inside(int x, int y) const;
    
protected:
    ItemID id = 0;      //  keeps track of this widget's ID wrt its parent
    SDL_Rect m_dimensions;
    Canvas* m_parent;   //  weak pointer    //  TODO: storing this here for now (might be useful for defining move semantics later)
    bool m_enabled;     //  whether an item should be able to interact with (useful for buttons)
    
    friend class Canvas;
};


/// destructor:
inline WidgetItem::~WidgetItem() = default;

/// modifiers:
inline WidgetItem& WidgetItem::pos(int x, int y) { m_dimensions.x = x; m_dimensions.y = y; return *this; }
inline WidgetItem& WidgetItem::size(int w, int h) { m_dimensions.w = w; m_dimensions.h = h; return *this; }

inline WidgetItem& WidgetItem::x(int x) { m_dimensions.x = x; return *this; }
inline WidgetItem& WidgetItem::y(int y) { m_dimensions.y = y; return *this; }
inline WidgetItem& WidgetItem::width(int width) { m_dimensions.w = width; return *this; }
inline WidgetItem& WidgetItem::height(int height) { m_dimensions.h = height; return *this; }

/// accessors:
inline int WidgetItem::x() const { return m_dimensions.x; }
inline int WidgetItem::y() const { return m_dimensions.y; }
inline int WidgetItem::width() const { return m_dimensions.w; }
inline int WidgetItem::height() const { return m_dimensions.h; }
inline Point WidgetItem::pos() const { return {x(), y()}; }
inline Size WidgetItem::size() const { return {width(), height()}; }
inline SDL_Rect WidgetItem::dimensions() const { return m_dimensions; }
inline bool WidgetItem::is_enabled() const { return m_enabled; }

/// GUI functions:
inline bool WidgetItem::handle_mouse_event(MouseEvent const& event)
{
    return is_enabled() && is_point_inside(event.pos.x, event.pos.y);
}
inline bool WidgetItem::handle_wheel_event(WheelEvent const& event)
{
    return is_enabled() && is_point_inside(event.pos.x, event.pos.y);
}
inline bool WidgetItem::handle_key_event(KeyEvent const& event)
{
    return false;
}

/// convenience functions:
inline bool WidgetItem::is_point_inside(int x, int y) const
{
    return (m_dimensions.x <= x && x <= m_dimensions.x + m_dimensions.w
            && m_dimensions.y <= y && y <= m_dimensions.y + m_dimensions.h);
}

inline void WidgetItem::enable() { m_enabled = true; }
inline void WidgetItem::disable() { m_enabled = false; }


#endif
