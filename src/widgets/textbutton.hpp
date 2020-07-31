/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#ifndef TEXTBUTTON_HPP
#define TEXTBUTTON_HPP

#include "button.hpp"
#include "textitem.hpp"


/**
 * @brief   Combines a button together with text
 *
 * @note    Handler Events: (inherited events)
 *
 * @note    Fonts will not be managed by this class.
 *          They should remain existent for the duration of the TextButton object.
 *
 * @note    Item will not be visible if font is not set.
 */
class TextButton : public Button, public TextItem
{
public:
    /// constructors:
    TextButton() noexcept;
    TextButton(SDL_Rect const&) noexcept;
    TextButton(TextButton const&);
    TextButton(TextButton&&) noexcept;
    
    /// destructor:
    virtual ~TextButton();
    
    /// assignment:
    TextButton& operator= (TextButton const&);
    TextButton& operator= (TextButton&&) noexcept;
    
    /// accessors:
    virtual bool is_visible() const override;
    
    /// GUI functions:
    virtual bool render(Renderer const&) const override;

    /// convenience functions:
    void swap(TextButton&) noexcept;
};


/// constructors:
inline TextButton::TextButton() noexcept : TextButton(SDL_Rect{0}) {}
inline TextButton::TextButton(SDL_Rect const& dimensions) noexcept : Button(dimensions), TextItem(ALIGN_CENTER) {}
inline TextButton::TextButton(TextButton const&) = default;
inline TextButton::TextButton(TextButton&& button) noexcept : TextButton{}
{
    //  providing a non-default implementation just
    //  so that we don't swap Item twice
    swap(button);
}

/// destructor:
inline TextButton::~TextButton() = default;

/// assignment:
inline TextButton& TextButton::operator= (TextButton const&) = default;
inline TextButton& TextButton::operator= (TextButton&& button) noexcept
{
    swap(button);
    return *this;
}

/// accessors:
inline bool TextButton::is_visible() const
{
    return TextItem::is_visible();
}

/// GUI functions:
inline bool TextButton::render(Renderer const& renderer) const
{
    //  draw button before text
    return Button::render(renderer) && TextItem::render(renderer);
}


#endif
