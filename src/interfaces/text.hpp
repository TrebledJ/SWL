/*
 *     Copyright (C) 2020 Johnathan Law
 *
 *     This file is part of SWL.
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


#ifndef INTERFACE_TEXT_HPP
#define INTERFACE_TEXT_HPP

#include "types.hpp"
#include "utility.hpp"
#include <string>


/**
 * @brief   A generic interface for text
 */
class TextInterface
{
public:
    /// constructors:
    TextInterface(Alignment = ALIGN_TOP_LEFT) noexcept;
    TextInterface(FontRef const&, Alignment = ALIGN_TOP_LEFT) noexcept;
    TextInterface(std::string const& text, FontRef const& = FontRef(), Alignment = ALIGN_TOP_LEFT) noexcept;
    
    /// destructor:
    virtual ~TextInterface();
    
    /// modifiers:
    void text(std::string const& text);
    void text(std::string const& text, Alignment);
    void text(std::string const& text, FontRef const&);
    void text(std::string const& text, FontRef const&, Alignment);
    void font(FontRef const& font);
    void align(Alignment alignment);
    
    /// accessors:
    virtual std::string const& text() const;
    
    /// @brief  Returns whether the whole text thing is valid (basically here, tests if font is not null)
    bool is_valid() const;
    
protected:
    std::string m_text;
    FontRef m_font;
    Alignment m_alignment;
};


/// constructors:
inline TextInterface::TextInterface(Alignment alignment) noexcept : TextInterface("", FontRef(), alignment) {}
inline TextInterface::TextInterface(FontRef const& font, Alignment alignment) noexcept : TextInterface("", font, alignment) {}
inline TextInterface::TextInterface(std::string const& text, FontRef const& font, Alignment alignment) noexcept
    : m_text(text)
    , m_font(font)
    , m_alignment(alignment)
{
}

/// destructor:
inline TextInterface::~TextInterface() = default;

/// modifiers:
inline void TextInterface::text(std::string const& text) { m_text = text; }
inline void TextInterface::text(std::string const& text, Alignment alignment) { m_text = text; m_alignment = alignment; }
inline void TextInterface::text(std::string const& text, FontRef const& font) { m_text = text; m_font = font; }
inline void TextInterface::text(std::string const& text, FontRef const& font, Alignment alignment) { m_text = text; m_font = font; m_alignment = alignment; }
inline void TextInterface::font(FontRef const& font) { m_font = font; }
inline void TextInterface::align(Alignment alignment) { m_alignment = alignment; }

/// accessors:
inline std::string const& TextInterface::text() const { return m_text; }
inline bool TextInterface::is_valid() const { return !m_font.expired(); }


#endif /* textinterface_hpp */
