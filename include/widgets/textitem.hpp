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

#ifndef textitem_hpp
#define textitem_hpp

#include "widgetitem.hpp"
#include "interfaces/text.hpp"
#include "types.hpp"
#include "utility.hpp"

#include <string>


class Canvas;

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
class TextItem : public WidgetItem, public TextInterface
{
public:
    /// constructors:
    TextItem(Canvas* parent = nullptr, std::string const& name = "") noexcept;
    TextItem(SDL_Rect const& dimensions, Canvas* parent = nullptr, std::string const& name = "") noexcept;
    TextItem(SDL_Rect const& dimensions, Alignment, Canvas* parent = nullptr, std::string const& name = "") noexcept;
    TextItem(TextItem const&) = delete;
    TextItem(TextItem&&) = delete;
    
    /// destructor:
    virtual ~TextItem();
    
    /// assignment:
    TextItem& operator= (TextItem const&) = delete;
    TextItem& operator= (TextItem&&) = delete;
    
    /// GUI functions:
    virtual void render(Renderer const&) const override;
};


/// constructors:
inline TextItem::TextItem(Canvas* parent, std::string const& name) noexcept
    : TextItem({0, 0, 0, 0}, ALIGN_TOP_LEFT, parent, name)
{
}
inline TextItem::TextItem(SDL_Rect const& dimensions, Canvas* parent, std::string const& name) noexcept
    : TextItem(dimensions, ALIGN_TOP_LEFT, parent, name)
{
}
inline TextItem::TextItem(SDL_Rect const& dimensions, Alignment alignment, Canvas* parent, std::string const& name) noexcept
    : WidgetItem(dimensions, parent, name)
    , TextInterface(alignment)
{
}

/// destructor:
inline TextItem::~TextItem() = default;

/// GUI functions:
inline void TextItem::render(Renderer const& renderer) const
{
    //  TODO: call show()/hide() when font is set? (do similar for DataView?)
    draw_text(renderer, m_dimensions, m_font.lock(), text(), m_alignment);
}


#endif
