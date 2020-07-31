/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#ifndef ITEM_HPP
#define ITEM_HPP

#include "types.hpp"
#include "sdl_inc.hpp"


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
    WidgetItem() noexcept;
    WidgetItem(SDL_Rect const&) noexcept;
    WidgetItem(WidgetItem const&) noexcept;
    WidgetItem(WidgetItem&&) noexcept;
    
    /// destructor:
    virtual ~WidgetItem();
    
    /// assignment:
    WidgetItem& operator= (WidgetItem const&) noexcept;
    WidgetItem& operator= (WidgetItem&&) noexcept;
    
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
    
    /**
     * @return  true if the button should be displayed/interacted with,
     *          false otherwise
     *
     * Override if other properties define an object's visibility
     *  or ability to interact.
     * OOP Note: Overrides of this function will affect other functions that use it.
     *  This includes the base implementations of handle_event() and render().
     *
     * See TextItem for an example of overriding is_visible().
     */
    virtual bool is_visible() const;
    
    /// GUI functions:
    /**
     * @brief   Delegates mouse/wheel events to their respective (i.e.
     *          handle_mouse_event() and handle_wheel_event()).
     *          Other events are handled here.
     * @return  true if the event was handled, false otherwise.
     *          For mouse events, an event is "handled" if the item is visible.
     *          For key events, an event is "handled" if the item responds to the key.
     *
     * On a normal basis, these function doesn't need to be overrided.
     * Override only when needed.
     *  (to introduce different interactive behaviours, delegating to children, etc.).
     *
     * See Button for an example of overriding handle_mouse_event()
     *  and Canvas for an example of overriding both events.
     *
     */
    virtual bool handle_mouse_event(MouseEvent const&);
    virtual bool handle_wheel_event(WheelEvent const&);
    virtual bool handle_key_event(KeyEvent const&);
    
    /**
     * @return  true if the item was rendered, false otherwise
     *
     * Override if there are other aspects to render (e.g. text, images)
     * or if there are other steps to take.
     *
     * See RectItem and Canvas for an example of overriding render().
     */
    virtual bool render(Renderer const&) const;
    
    /// convenience functions:
    void swap(WidgetItem&) noexcept;
    
    /**
     * @brief   Checks if the point x, y is inside the dimensions
     *          (treated as global relative to window)
     */
    bool is_point_inside(int x, int y) const;
    
protected:
    SDL_Rect m_dimensions;
    bool m_visible;     //  whether an item should be seen
    bool m_enabled;     //  whether an item should be able to interact with (useful for buttons)
};


/// constructors:
inline WidgetItem::WidgetItem() noexcept : WidgetItem(SDL_Rect{0}) {}
inline WidgetItem::WidgetItem(SDL_Rect const& dimensions) noexcept : m_dimensions{dimensions}, m_visible{true}, m_enabled{true} {}
inline WidgetItem::WidgetItem(WidgetItem const&) noexcept = default;
inline WidgetItem::WidgetItem(WidgetItem&& item) noexcept { swap(item); item.m_visible = false; }

/// destructor:
inline WidgetItem::~WidgetItem() = default;

/// assignment:
inline WidgetItem& WidgetItem::operator= (WidgetItem const&) noexcept = default;
inline WidgetItem& WidgetItem::operator= (WidgetItem&& item) noexcept
{
    swap(item);
    item.m_visible = false;
    return *this;
}

/// modifiers:
inline WidgetItem& WidgetItem::pos(int x, int y) { m_dimensions.x = x; m_dimensions.y = y; return *this; }
inline WidgetItem& WidgetItem::size(int w, int h) { m_dimensions.w = w; m_dimensions.h = h; return *this; }

inline WidgetItem& WidgetItem::x(int x) { m_dimensions.x = x; return *this; }
inline WidgetItem& WidgetItem::y(int y) { m_dimensions.y = y; return *this; }
inline WidgetItem& WidgetItem::width(int width) { m_dimensions.w = width; return *this; }
inline WidgetItem& WidgetItem::height(int height) { m_dimensions.h = height; return *this; }

inline void WidgetItem::show() { m_visible = true; }
inline void WidgetItem::hide() { m_visible = false; }

/// accessors:
inline int WidgetItem::x() const { return m_dimensions.x; }
inline int WidgetItem::y() const { return m_dimensions.y; }
inline int WidgetItem::width() const { return m_dimensions.w; }
inline int WidgetItem::height() const { return m_dimensions.h; }
inline Point WidgetItem::pos() const { return {x(), y()}; }
inline Size WidgetItem::size() const { return {width(), height()}; }
inline SDL_Rect WidgetItem::dimensions() const { return m_dimensions; }
inline bool WidgetItem::is_visible() const { return m_visible; }

/// GUI functions:
inline bool WidgetItem::handle_mouse_event(MouseEvent const& event)
{
    return is_visible() && m_enabled && is_point_inside(event.pos.x, event.pos.y);
}
inline bool WidgetItem::handle_wheel_event(WheelEvent const& event)
{
    return is_visible() && m_enabled && is_point_inside(event.pos.x, event.pos.y);
}
inline bool WidgetItem::handle_key_event(KeyEvent const& event)
{
    return false;
}

inline bool WidgetItem::render(Renderer const& renderer) const { return is_visible(); }

/// convenience functions:
inline bool WidgetItem::is_point_inside(int x, int y) const
{
    return (m_dimensions.x <= x && x <= m_dimensions.x + m_dimensions.w
            && m_dimensions.y <= y && y <= m_dimensions.y + m_dimensions.h);
}

inline void WidgetItem::enable() { m_enabled = true; }
inline void WidgetItem::disable() { m_enabled = false; }


#endif