/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "widgetitem.hpp"
#include "types.hpp"
#include "utility.hpp"

#include "sdl_image_inc.hpp"
#include <string>


/**
 * @brief   Represents an image and manages the underlying texture
 */
class ImageItem : public WidgetItem
{
public:
    /// constructors:
    ImageItem() noexcept;
    ImageItem(Renderer const&, std::string const& filename);
    ImageItem(ImageItem const&) = delete;
    ImageItem(ImageItem&&) noexcept;
    
    /// destructor:
    virtual ~ImageItem();
    
    /// assignment:
    ImageItem& operator= (ImageItem const&) = delete;
    ImageItem& operator= (ImageItem&&) noexcept;
    
    /// modifiers:
    bool load(Renderer const&, std::string const& filename);
    
    /// accessors:
    virtual bool is_visible() const override;
    
    /// GUI functions:
    virtual bool render(Renderer const&) const override;
    
    /// convenience functions:
    void swap(ImageItem&) noexcept;
    
private:
    Texture m_texture;
};


/// constructors:
inline ImageItem::ImageItem() noexcept = default;
inline ImageItem::ImageItem(Renderer const& renderer, std::string const& filename)
{
    load(renderer, filename);
}
inline ImageItem::ImageItem(ImageItem&& other) noexcept
{
    swap(other);
}

/// assignment:
inline ImageItem& ImageItem::operator= (ImageItem&& other) noexcept
{
    swap(other);
    return *this;
}

/// modifiers:
inline bool ImageItem::load(Renderer const& renderer, std::string const& filename)
{
    return bool( m_texture = make_texture(IMG_LoadTexture(renderer.get(), filename.data())) );
}

/// accessors:
inline bool ImageItem::is_visible() const
{
    return WidgetItem::is_visible() && m_texture;
}


#endif
