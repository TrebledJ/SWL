/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#ifndef RECTITEM_HPP
#define RECTITEM_HPP

#include "widgetitem.hpp"
#include "themes.hpp"
#include "types.hpp"

#include "sdl_inc.hpp"


/**
 * @brief   Displays color. Useful for backgrounds.
 */
class RectItem : public virtual WidgetItem
{
public:
    /// constructors:
    RectItem() noexcept;
    RectItem(SDL_Rect const&) noexcept;
    RectItem(RectItem const&) noexcept;
    RectItem(RectItem&&) noexcept;
    
    /// destructor:
    virtual ~RectItem();
    
    /// assignment:
    RectItem& operator= (RectItem const&) noexcept;
    RectItem& operator= (RectItem&&) noexcept;
    
    /// modifiers:
    RectItem& background(SDL_Color const& color);
    
    /// GUI functions:
    virtual bool render(Renderer const& renderer) const override;
    
protected:
    SDL_Color m_background_color;
};


/// constructors:
inline RectItem::RectItem() noexcept : RectItem({0, 0, 0, 0}) {}
inline RectItem::RectItem(SDL_Rect const& dimensions) noexcept : m_background_color{Themes::PRIMARY} {
    m_dimensions = dimensions;  //  since WidgetItem is a virtual base class, use direct assignment
}
inline RectItem::RectItem(RectItem const&) noexcept = default;
inline RectItem::RectItem(RectItem&& item) noexcept
    : WidgetItem{std::move(item)}
{
    std::swap(m_background_color, item.m_background_color);
}

/// destructor:
inline RectItem::~RectItem() = default;

/// assignment:
inline RectItem& RectItem::operator= (RectItem const&) noexcept = default;
inline RectItem& RectItem::operator= (RectItem&& item) noexcept
{
    std::swap(*this, item);
    std::swap(m_background_color, item.m_background_color);
    return *this;
}

/// modifiers:
inline RectItem& RectItem::background(SDL_Color const& color)
{
    m_background_color = color;
    return *this;
}


#endif
