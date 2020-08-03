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

#ifndef LISTVIEW_HPP
#define LISTVIEW_HPP

#include "dataview.hpp"
#include "listmodel.hpp"
#include "button.hpp"
#include "utility.hpp"

#include "types.hpp"
#include "themes.hpp"

#include <functional>
#include <string>
#include <type_traits>
#include <vector>


const bool DEBUG_LISTVIEW = false;


//  TODO: Add sort-by-header functionality      
//  TODO: Add support for single-selection and multi-selection (shift-click)
//  TODO: Add scroll bar option
//  TODO: Add getters (if they are needed?)


/**
 * @brief   A widget used to present and interact with data in a list format.
 *          This implements the View and Controller of the MVC design pattern.
 *
 * @note    Handler Events: (inherited events)
 * @note    Use in conjunction with any DataModel. Items should inherit ListItem.
 *
 * @note    Models, items, and fonts will not be managed by this class.
 *          They should remain existent while the ListView object is in use.
 *
 * @note    If both index_clicked and item_clicked handlers are defined, index_clicked is called first.
 * @note    At most header.size() columns will be displayed.
 * @note    Columns with ratios of 0 will be treated as 0 width.
 * @note    The default ratio for columns is 1.
 */
template<class T, typename = typename std::enable_if<std::is_base_of<ListItem, T>::value>::type>
class ListView;

template<class T>
    /* requires derived_from<T, ListItem> */
class ListView<T> : public DataView<T>
{
    using Super = DataView<T>;
public:
    /// constructors:
    ListView(Canvas* parent = nullptr) noexcept;
    ListView(SDL_Rect const&, Canvas* parent = nullptr) noexcept;
    ListView(ListView&&) = delete;

    /// destructors:
    ~ListView();
    
    /// assignment:
    ListView& operator= (ListView&&) = delete;

    /// modifiers:
    ListView& headers(std::vector<std::string> const& headers);
    ListView& column_ratios(std::vector<unsigned> const& ratios);
    ListView& header_font(FontRef const& font);
    ListView& header_height(int height);
    ListView& selection_color(SDL_Color const& color);
    ListView& draw_item_borders(bool draw = true);
    
    /// accessors:
    virtual bool is_visible() const override;
    
    /// GUI functions:
    virtual bool render(Renderer const& renderer) const override;
    
    /// convenience functions:
    void swap(ListView& other) noexcept;
    
private:
    static const unsigned DEFAULT_COLUMN_RATIO = 1;
    
    std::vector<std::string> m_headers;
    std::vector<unsigned> m_column_ratios;
    FontRef m_header_font;
    int m_header_height;
    SDL_Color m_selection_color;
    bool m_draw_item_borders;
    
private:
    /// render helper functions:
    void render_head(Renderer const& renderer) const;
    virtual void render_item(Renderer const& renderer, T const& item, SDL_Rect const& bounds) const override;
    
    /// general helper functions:
    virtual int internal_height() const override;

    //  this... may or may not change in the future...
    unsigned columns() const;
    
    /// @return The x of the column at column `index`
    int x_at(std::size_t col_index) const;

    virtual int y0() const override;
    
    int width_at(std::size_t col_index) const;
    unsigned ratio_at(std::size_t col_index) const;
    unsigned total_column_ratio() const;
    
    /// convenience functions:
    void swap_members(ListView& other) noexcept;
};


/// constructors:
template<class T>
inline ListView<T>::ListView(Canvas* parent) noexcept
    : ListView<T>({0, 0, 0, 0}, parent)
{
}
template<class T>
inline ListView<T>::ListView(SDL_Rect const& dimensions, Canvas* parent) noexcept
    : Super(dimensions, parent)
    , m_header_height{60}
    , m_selection_color{Colors::LIGHT_GREEN}
    , m_draw_item_borders{false}
{
}

/// destructor:
template<class T>
inline ListView<T>::~ListView() = default;

/// modifiers:
template<class T>
inline ListView<T>& ListView<T>::headers(std::vector<std::string> const& headers) { m_headers = headers; return *this; }
template<class T>
inline ListView<T>& ListView<T>::column_ratios(std::vector<unsigned> const& ratios) { m_column_ratios = ratios; return *this; }
template<class T>
inline ListView<T>& ListView<T>::header_font(FontRef const& font) { m_header_font = font; return *this; }
template<class T>
inline ListView<T>& ListView<T>::header_height(int height) { m_header_height = height; return *this; }
template<class T>
inline ListView<T>& ListView<T>::selection_color(SDL_Color const& color) { m_selection_color = color; return *this; }
template<class T>
inline ListView<T>& ListView<T>::draw_item_borders(bool draw) { m_draw_item_borders = draw; return *this; }

/// accessors:
template<class T>
inline bool ListView<T>::is_visible() const
{
    return Super::is_visible() && !m_header_font.expired();
}

/// GUI functions:
template<class T>
bool ListView<T>::render(Renderer const& renderer) const
{
    if (!Super::render(renderer))
        return false;
    
    render_head(renderer);
    this->render_body(renderer);
    
    if (DEBUG_LISTVIEW)
    {
        //  draw outline inside margins
        SDL_Rect bounds = {x_at(0), this->m_dimensions.y + this->m_margins.left,
            this->internal_width(), internal_height()};
        draw_rect(renderer, bounds, Colors::RED);
    }

    return true;
}

/// convenience functions:
template<class T>
void ListView<T>::swap(ListView& other) noexcept
{
    this->swap(other);
    swap_members(other);
}

/// render helper functions:
template<class T>
void ListView<T>::render_head(Renderer const& renderer) const
{
    const auto y = DataView<T>::y0();
    for (auto c = 0; c < columns(); ++c)
        draw_text(renderer, {x_at(c), y, 0, m_header_height},
                  m_header_font.lock(), m_headers[c], ALIGN_CENTER_LEFT);

    if (DEBUG_LISTVIEW)
    {
        //  draw outline
        SDL_Rect bounds = {x_at(0), y, this->internal_width(), m_header_height};
        draw_rect(renderer, bounds, Colors::BLACK);
    }
}

template<class T>
void ListView<T>::render_item(Renderer const& renderer, T const& item, SDL_Rect const& bounds) const
{
    //  draw background color of selected item
    if (item.selected)
        draw_filled_rect(renderer, bounds, m_selection_color);

    //  draw model data
    for (auto col = 0; col < std::min(columns(), static_cast<unsigned>(item.fields())); ++col)
        draw_text(renderer,
                  {x_at(col) + this->m_item_padding.left, bounds.y + this->m_item_padding.top,
                    bounds.w, bounds.h},
                  this->m_item_font.lock(), item.field_at(col), ALIGN_CENTER_LEFT);

    if (m_draw_item_borders)
        draw_rect(renderer, bounds, Colors::BLACK);
    
    if (DEBUG_LISTVIEW)
    {
        //  draw outline
        draw_rect(renderer, bounds, Colors::BLACK);
    }
}

/// general helper functions:
template<class T>
inline int ListView<T>::internal_height() const { return DataView<T>::internal_height() - m_header_height; }

template<class T>
inline unsigned ListView<T>::columns() const { return static_cast<unsigned>(m_headers.size()); }

template<class T>
int ListView<T>::x_at(std::size_t col_index) const
{
    int x = this->m_dimensions.x + this->m_margins.left;
    for (auto i = 0; i < col_index; ++i)
        x += width_at(i);
    return x;
}

template<class T>
inline int ListView<T>::y0() const
{
    return DataView<T>::y0() + m_header_height;
}

template<class T>
inline int ListView<T>::width_at(std::size_t col_index) const
{
    auto ratio = ratio_at(col_index);
    return ratio == 0 ? 0 : this->internal_width() * (float(ratio) / total_column_ratio());
}

template<class T>
inline unsigned ListView<T>::ratio_at(std::size_t col_index) const
{
    if (col_index >= columns())
        return 0u;
    return col_index < m_column_ratios.size() ? m_column_ratios[col_index] : DEFAULT_COLUMN_RATIO;
}

template<class T>
inline unsigned ListView<T>::total_column_ratio() const
{
    unsigned total = 0;
    for (auto i = 0; i < std::min(columns(), static_cast<unsigned>(m_column_ratios.size())); ++i)
        total += m_column_ratios[i];
    if (m_column_ratios.size() < columns())
        total += (columns() - m_column_ratios.size()) * DEFAULT_COLUMN_RATIO;
    return total;
}

template<class T>
void ListView<T>::swap_members(ListView& other) noexcept
{
    std::swap(m_headers, other.m_headers);
    std::swap(m_column_ratios, other.m_column_ratios);
    std::swap(m_header_font, other.m_header_font);
    std::swap(m_header_height, other.m_header_height);
    std::swap(m_draw_item_borders, other.m_draw_item_borders);
}


#endif
