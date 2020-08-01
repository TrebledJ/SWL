/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#ifndef textitem_hpp
#define textitem_hpp

#include "widgetitem.hpp"
#include "types.hpp"
#include "utility.hpp"

#include <string>


/**
 * @brief   Displays text
 *
 * @note    Handler Events: (inherited events)
 *
 * @note    Fonts will not be managed by this class.
 *          They should remain existent for the duration of the TextButton object.
 *
 * @note    Item will not be visible if font is not set.
 * @note    If move constructor/move assignment/destructor fails (i.e. terminates),
 *          it is likely caused by the deleter for FontRef. Make sure to call TTF_Quit()
 *          AFTER the TextItem objects are destructed (if TTF_Quit is used at all).
 *          The Application class handles this by default.
 */
class TextItem : public virtual WidgetItem
{
public:
    /// constructors:
    TextItem(Alignment = ALIGN_TOP_LEFT) noexcept;
    TextItem(SDL_Rect const&, Alignment = ALIGN_TOP_LEFT) noexcept;
    TextItem(TextItem const&);
    TextItem(TextItem&&) noexcept;
    
    /// destructor:
    virtual ~TextItem();
    
    /// assignment:
    TextItem& operator= (TextItem const&);
    TextItem& operator= (TextItem&&) noexcept;
    
    /// modifiers:
    TextItem& font(FontRef const& font);
    TextItem& text(std::string const& text);
    TextItem& align(Alignment alignment);
    
    /// accessors:
    std::string const& text() const;
    virtual bool is_visible() const override;
    
    /// GUI functions:
    virtual bool render(Renderer const&) const override;
    
    /// convenience functions:
    void swap(TextItem&) noexcept;

protected:
    FontRef m_font;
    std::string m_text;
    Alignment m_alignment;
    
protected:
    void swap_members(TextItem&) noexcept;
};


/// constructors:
inline TextItem::TextItem(Alignment alignment) noexcept : TextItem({0, 0, 0, 0}, alignment) {}
inline TextItem::TextItem(SDL_Rect const& rect, Alignment alignment) noexcept : WidgetItem(rect), m_alignment{alignment} {}
inline TextItem::TextItem(TextItem const&) = default;
inline TextItem::TextItem(TextItem&& item) noexcept : TextItem{}
{
    swap(item);
    item.m_font.reset();
}

/// destructor:
inline TextItem::~TextItem() = default;

/// assignment:
inline TextItem& TextItem::operator= (TextItem const&) = default;
inline TextItem& TextItem::operator= (TextItem&& item) noexcept
{
    swap(item);
    item.m_font.reset();
    return *this;
}

/// modifiers:
inline TextItem& TextItem::font(FontRef const& font) { m_font = font; return *this; }
inline TextItem& TextItem::text(std::string const& text) { m_text = text; return *this; }
inline TextItem& TextItem::align(Alignment alignment) { m_alignment = alignment; return *this; }

/// accessors:
inline std::string const& TextItem::text() const { return m_text; }
inline bool TextItem::is_visible() const { return WidgetItem::is_visible() && !m_font.expired(); }


#endif
