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

#ifndef DATAVIEW_HPP
#define DATAVIEW_HPP

#include "models/datamodel.hpp"
#include "widgets/widgetitem.hpp"
#include "interfaces/button.hpp"

#include <string>
#include <vector>


class Canvas;

/**
 * @brief   An abstract base class to render a data model.
 *          View + Controller: knows nothing about the data,
 *                             only about visualisation and user interaction.
 *
 * @note    Inherited classes will need to implement render_item().
 *
 * @note    Handler events: SDL_MOUSEBUTTONDOWN, SDL_MOUSEWHEEL, (inherited events)
 * @note    Inherited classes may also override internal_width(),
 *          internal_height(), y0()
 */
template<class T>
class DataView : public RectItem, public ButtonInterface
{
    using Super = RectItem;
    
public:
    using IndexCallback = std::function<void(int index)>;
    using WheelEventCallback = std::function<void(WheelEvent const&)>;

public:
    /// constructors:
    DataView(Canvas* parent = nullptr, std::string const& id = "") noexcept;
    DataView(SDL_Rect const&, Canvas* parent = nullptr, std::string const& id = "") noexcept;

    //  copy-ctor and copy-asgn are deleted so that properties are EXPLICIT
    //  (either from move funcs or modifiers) and so that there will be no
    //  complications with copying button handlers
    DataView(DataView const& other) = delete;
    DataView(DataView&&) = delete;

    /// destructor:
    virtual ~DataView();
    
    /// assignment:
    DataView& operator= (DataView const& other) = delete;
    DataView& operator= (DataView&&) = delete;

    /// modifiers:
    DataView& model(DataModel<T>* model);
    DataView& item_font(FontRef const& font);
    DataView& margins(Margins const& margins);
    DataView& item_padding(Padding const& padding);
    DataView& item_height(int height);
    DataView& on_scrolled(WheelEventCallback);
    DataView& on_index_clicked(IndexCallback);
    DataView& on_index_hovered(IndexCallback);
    
    /// accessors:
    bool is_valid() const;
    
    /// GUI functions:
    virtual bool handle_mouse_event(MouseEvent const& event) override;
    virtual bool handle_wheel_event(WheelEvent const& event) override;
    virtual void render(Renderer const& renderer) const override;
    
    /// convenience functions:
    void swap(DataView& other) noexcept;
    
protected:
    static const unsigned DEFAULT_COLUMN_RATIO = 1;
    
    const DataModel<T>* m_model;  //  weak pointer
    FontRef m_item_font;
    SDL_Color m_item_color;
    Margins m_margins;
    Padding m_item_padding;
    int m_item_height;
    int m_display_index;
    
protected:
    /// general helper functions:
    virtual int internal_width() const;
    virtual int internal_height() const;

    /// @return The y of the first row
    virtual int y0() const;
    /// @return The y of the row at row `index`
    int y_at(std::size_t row_index) const;
    
    /// @return The maximum number of items that could be displayed (fit) on the listview
    int get_nb_display_items() const;
    
    /**
     * @return the index under the given coordinates, -1 if no valid indices are under
     *
     * TODO: In C++17, this would be better implemented as returning std::optional<std::size_t>.
     */
    int get_index_under(int x, int y) const;
    
    /// render helper functions:
    /// @brief  Iterates through model and renders items that can be seen
    void render_body(Renderer const&) const;
    /// @brief  Renders the item from the model at the given index
    virtual void render_item(Renderer const&, T const& item, SDL_Rect const& bounds) const = 0;
    
private:
    WheelEventCallback m_scrolled;
    IndexCallback m_index_clicked;
    IndexCallback m_index_hovered;

    /// convenience functions:
    void swap_members(DataView& other) noexcept;
};


/// constructors:
template<class T>
inline DataView<T>::DataView(Canvas* parent, std::string const& id) noexcept
    : DataView<T>({0, 0, 0, 0}, parent, id)
{
}
template<class T>
inline DataView<T>::DataView(SDL_Rect const& dimensions, Canvas* parent, std::string const& id) noexcept
    : Super(dimensions, parent, id)
    , m_model{nullptr}
    , m_item_height{40}
    , m_display_index{0}
{
}

/// destructor:
template<class T>
inline DataView<T>::~DataView() = default;

/// modifiers:
template<class T>
inline DataView<T>& DataView<T>::model(DataModel<T>* model) { m_model = model; return *this; }
template<class T>
inline DataView<T>& DataView<T>::item_font(FontRef const& font) { m_item_font = font; return *this; }
template<class T>
inline DataView<T>& DataView<T>::margins(Margins const& margins) { m_margins = margins; return *this; }
template<class T>
inline DataView<T>& DataView<T>::item_padding(Padding const& padding) { m_item_padding = padding; return *this; }
template<class T>
inline DataView<T>& DataView<T>::item_height(int height) { m_item_height = height; return *this; }
template<class T>
inline DataView<T>& DataView<T>::on_scrolled(WheelEventCallback f) { m_scrolled = f; return *this; }
template<class T>
inline DataView<T>& DataView<T>::on_index_clicked(IndexCallback f) { m_index_clicked = f; return *this; }
template<class T>
inline DataView<T>& DataView<T>::on_index_hovered(IndexCallback f) { m_index_hovered = f; return *this; }

/// accessors:
template<class T>
inline bool DataView<T>::is_valid() const
{
    return m_model && !m_item_font.expired();
}

/// GUI functions:
template<class T>
bool DataView<T>::handle_mouse_event(MouseEvent const& event)
{
    if (!Super::handle_mouse_event(event) || !is_valid())
        return false;
    
    auto index = get_index_under(event.pos.x, event.pos.y);
    if (event.type == MouseEvent::UP)
    {
        if (m_index_clicked) m_index_clicked(index);
    }
    if (event.type == MouseEvent::MOTION)
    {
        if (m_index_hovered) m_index_hovered(index);
    }
    
    return true;
}

template<class T>
bool DataView<T>::handle_wheel_event(WheelEvent const& event)
{
    if (!Super::handle_wheel_event(event) || !is_valid())
        return false;
    
    int delta = event.wheel.y;
    if (delta == 0)
        return true;
    
    int max_display_index = std::max(int(m_model->rows()) - get_nb_display_items(), 0);
    m_display_index -= delta;
    m_display_index = std::min(std::max(m_display_index, 0), max_display_index);
    if (m_scrolled) m_scrolled(event);
    return true;
}

template<class T>
void DataView<T>::render(Renderer const& renderer) const
{
    if (!is_valid())
        return;
    
    Super::render(renderer);
    render_body(renderer);
}

/// convenience functions:
template<class T>
void DataView<T>::swap(DataView& other) noexcept
{
    Super::swap(other);
    swap_members(other);
}

/// general helper functions:
template<class T>
inline int DataView<T>::internal_width() const { return m_dimensions.w - m_margins.left - m_margins.right; }
template<class T>
inline int DataView<T>::internal_height() const { return m_dimensions.h - m_margins.top - m_margins.bottom; }

template<class T>
inline int DataView<T>::get_nb_display_items() const
{
    return internal_height() / m_item_height;
}

template<class T>
inline int DataView<T>::y_at(std::size_t row_index) const
{
    return y0() + static_cast<int>(m_item_height * row_index);
}

template<class T>
inline int DataView<T>::y0() const
{
    return m_dimensions.y + m_margins.top;
}

template<class T>
int DataView<T>::get_index_under(int x, int y) const
{
    if (x < m_dimensions.x + m_margins.left || x > m_dimensions.x + m_dimensions.w - m_margins.right)
        return -1;

    float index_f = float(y - y0()) / m_item_height;
    if (index_f < 0.f || index_f >= get_nb_display_items())
        return -1;

    int index = m_display_index + int(index_f);
    if (index >= m_model->rows())
        return -1;  // for the case where #items < #max_display_items && clicking on an the excess area
    
    return index;
}

template<class T>
void DataView<T>::render_body(Renderer const& renderer) const
{
    const auto nb_display = get_nb_display_items();
    const auto max_row = std::min(m_model->rows(), static_cast<std::size_t>(m_display_index + nb_display));
    
    for (auto row = m_display_index; row < max_row; ++row)
    {
        auto y = y_at(row - m_display_index);
        SDL_Rect bounds = {m_dimensions.x + m_margins.left, y, internal_width(), m_item_height};
        
        render_item(renderer, m_model->at(row), bounds);
    }
}

template<class T>
void DataView<T>::swap_members(DataView& other) noexcept
{
    std::swap(m_model, other.m_model);
    std::swap(m_item_font, other.m_item_font);
    std::swap(m_margins, other.m_margins);
    std::swap(m_item_height, other.m_item_height);
    std::swap(m_scrolled, other.m_scrolled);
    std::swap(m_index_clicked, other.m_index_clicked);
    std::swap(m_index_hovered, other.m_index_hovered);
}


#endif
