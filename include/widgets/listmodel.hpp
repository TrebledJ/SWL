/*
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

#ifndef LISTMODEL_HPP
#define LISTMODEL_HPP

#include "datamodel.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <type_traits>
#include <vector>


class ListItem
{
public:
    bool selected = false;
    
    ListItem() noexcept = default;
    virtual ~ListItem() = default;
    
    /**
     * @brief   Returns the number of fields in the item.
     */
    virtual std::size_t fields() const = 0;
    
    /**
     * @brief   Returns a value at a particular index.
     *          Valid values should be returned for any index within the range [0, size[
     */
    virtual std::string field_at(int index) const = 0;
};


/**
 * @brief   A model for storing data as a list of items.
 *
 * @note    Use in conjunction with ListView.
 * @note    Items should inherit ListItem.
 */
template<class T, typename = typename std::enable_if<std::is_base_of<ListItem, T>::value>::type>
class ListModel;

template<class T>
class ListModel<T> : public DataModel<T>
{
public:
    using Comparator = std::function<bool(T const&, T const&)>;
    using Partitioner = std::function<bool(T const&)>;
    
public:
    /// modifiers:
    ListModel& add(T const& item);
    ListModel& add_items(std::vector<T> const& items);
    ListModel& remove(std::size_t index);
    ListModel& remove_selected_items();
    
    /// @brief  Clears the model
    ListModel& clear();
    
    /**
     * @brief   Sets a persistent comparator and sorts once
     */
    ListModel& sorter(Comparator cmp);

    void select(std::size_t index);
    void unselect(std::size_t index);
    void toggle_select(std::size_t index);
    
    /**
     * @param cmp   A comparison function that satisfies strict weak ordering
     *              (i.e. returns true if the first element is less than the second
     *              and false otherwise).
     */
    void sort_once(Comparator cmp = nullptr);
    
    /**
     * @brief   Partitions the model. Relative order of items is preserved
     */
    void partition_once(Partitioner cmp);
    
    /// accessors:
    std::size_t rows() const;
    T const& at(std::size_t index) const;
    T        at(std::size_t index);
    T const& operator[] (std::size_t index) const;
    T        operator[] (std::size_t index);
    
    int count_selected_items() const;
    std::vector<T> selected_items() const;
    
private:
    std::vector<T> m_items;
    Comparator m_cmp;
    
private:
    /**
     * @brief   Adds or insertion-sort-inserts the item
     */
    void add_if_can_sort(T const& item);
    
    /**
     * @brief   Recursively searches items for the index where the item should be inserted
     * @return  The index to insert the element at
     */
    std::size_t binary_search(T const& item, std::size_t low, std::size_t high) const;
    
};


/// modifiers:
template<class T>
inline ListModel<T>& ListModel<T>::add(T const& item) { add_if_can_sort(item); return *this; }

template<class T>
ListModel<T>& ListModel<T>::add_items(std::vector<T> const& items)
{
    m_items.reserve(m_items.size() + items.size());
    for (auto const& item : items)
        add_if_can_sort(item);
    return *this;
}

template<class T>
ListModel<T>& ListModel<T>::remove(std::size_t index)
{
    if (index < m_items.size())
        m_items.erase(m_items.begin() + index);
    return *this;
}

template<class T>
ListModel<T>& ListModel<T>::remove_selected_items()
{
    int correction = 0;
    for (auto i = 0; i - correction < m_items.size(); ++i)
        if (m_items[i - correction].selected)
        {
            m_items.erase(m_items.begin() + i - correction);
            correction++;
        }
    return *this;
}

template<class T>
inline ListModel<T>& ListModel<T>::clear()
{
    m_items.clear();
    return *this;
}

template<class T>
ListModel<T>& ListModel<T>::sorter(Comparator cmp)
{
    m_cmp = cmp;
    sort_once();
    return *this;
}

template<class T>
inline void ListModel<T>::select(std::size_t index) { m_items.at(index).selected = true; }
template<class T>
inline void ListModel<T>::unselect(std::size_t index) { m_items.at(index).selected = false; }
template<class T>
inline void ListModel<T>::toggle_select(std::size_t index)
{
    if (0 <= index && index < m_items.size())
        m_items[index].selected = !m_items[index].selected;
}

template<class T>
inline void ListModel<T>::sort_once(Comparator cmp)
{
    if (cmp || m_cmp)
        std::sort(m_items.begin(), m_items.end(), cmp ? cmp : m_cmp);
}

template<class T>
inline void ListModel<T>::partition_once(Partitioner cmp)
{
    std::stable_partition(m_items.begin(), m_items.end(), cmp);
}

/// accessors:
template<class T>
inline std::size_t ListModel<T>::rows() const { return m_items.size(); }
template<class T>
inline T const& ListModel<T>::at(std::size_t index) const { return m_items.at(index); }
template<class T>
inline T        ListModel<T>::at(std::size_t index) { return m_items.at(index); }
template<class T>
inline T const& ListModel<T>::operator[] (std::size_t index) const { return m_items[index]; }
template<class T>
inline T        ListModel<T>::operator[] (std::size_t index) { return m_items[index]; }

template<class T>
int ListModel<T>::count_selected_items() const
{
    int counter = 0;
    for (auto const& item : m_items)
        if (item.selected)
            counter++;
    return counter;
}
template<class T>
std::vector<T> ListModel<T>::selected_items() const
{
    std::vector<T> items;
    for (auto i = 0; i < m_items.size(); ++i)
        if (m_items[i].selected)
            items.push_back(m_items[i]);
    return items;
}

template<class T>
void ListModel<T>::add_if_can_sort(T const& item)
{
    if (!m_cmp || m_items.empty())
    {
        m_items.push_back(item);
        return;
    }
    
    //  insertion-sort insert
    auto idx = binary_search(item, 0, m_items.size()-1);
    m_items.insert(m_items.begin() + idx, item);
}

//  note: the following implementation is inspired from a combination of the following:
//   (1) https://www.geeksforgeeks.org/binary-insertion-sort/
//   (2) https://en.wikipedia.org/wiki/Binary_search_algorithm#Alternative_procedure (used because of the restriction of one comparator)
template<class T>
std::size_t ListModel<T>::binary_search(T const& item, std::size_t low, std::size_t high) const
{
    if (high == low)
        return !m_cmp(item, m_items[low]) ? low+1 : low;
    
    //  note: at this point, low + high > 0,
    //        hence mid > 0 and mid-1 will not underflow
    std::size_t mid = std::ceil((low + high) / 2.0);
    return m_cmp(item, m_items[mid]) ? binary_search(item, low, mid-1) : binary_search(item, mid, high);
}


#endif
