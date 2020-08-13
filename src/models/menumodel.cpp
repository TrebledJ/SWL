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

#include "models/menumodel.hpp"


const std::string MenuModel::BACK = "Back";


MenuNode::MenuNode(std::string const& text)
    : text{text}
    , m_parent{nullptr}
{
}

/// modifiers:
MenuNode* MenuNode::add(std::string const& text)
{
    auto node = new MenuNode{text};
    node->m_parent = this;
    m_children.push_back(std::unique_ptr<MenuNode>(node));
    return node;
}

void MenuNode::clear()
{
    m_children.clear();
}

/// accessors:
MenuNode* MenuNode::get(std::size_t index) const
{
    if (!(0 <= index && index < m_children.size()))
        return nullptr;
    return m_children.at(index).get();
}

MenuNode* MenuNode::get(std::string const& text) const
{
    for (auto const& node : m_children)
        if (node->text == text)
            return node.get();
    
    return nullptr;
}

MenuNode* MenuNode::parent() const
{
    return m_parent;
}

std::size_t MenuNode::size() const
{
    return m_children.size();
}

bool MenuNode::is_final() const
{
    return m_children.empty();
}


MenuModel::MenuModel() noexcept
    : m_root_node{nullptr}
    , m_current_node{nullptr}
    , m_back_navigation{false}
{
    m_current_node = new MenuNode;
    m_root_node = std::unique_ptr<MenuNode>(m_current_node);
}

/// modifiers:
MenuNode* MenuModel::add(std::string const& text)
{
    return m_current_node->add(text);
}

void MenuModel::clear()
{
    m_current_node->clear();
}

void MenuModel::back_navigation(bool on)
{
    m_back_navigation = on;
}

void MenuModel::go_to_root()
{
    m_current_node = m_root_node.get();
}

bool MenuModel::go_to_parent()
{
    if (!m_current_node->parent())
        return false;
    
    m_current_node = m_current_node->parent();
    return true;
}

bool MenuModel::go_to_index(std::size_t index)
{
    if (!m_current_node)
        return false;
    
    auto node = m_current_node->get(index);
    if (!node)
        return false;
    
    m_current_node = node;
    return true;
}

bool MenuModel::go_to_option(std::string const& text)
{
    if (!m_current_node)
        return false;
    
    auto node = m_current_node->get(text);
    if (!node)
        return false;
    
    m_current_node = node;
    return true;
}

/// accessors:
std::size_t MenuModel::rows() const
{
    return m_current_node->size() + has_back();
}

std::string const& MenuModel::at(std::size_t index) const
{
    static const std::string empty = "";
    if (!valid(index))
        return empty;
    
    return has_back() && index == rows() - 1 ? BACK : m_current_node->get(index)->text;
}

bool MenuModel::is_final(std::size_t index) const
{
    return valid(index) ? m_current_node->get(index)->is_final() : false;
}

bool MenuModel::is_root() const
{
    return m_current_node == root();
}

MenuNode* MenuModel::root() const
{
    return m_root_node.get();
}

MenuNode* MenuModel::node() const
{
    return m_current_node;
}

MenuNode* MenuModel::node_at(std::size_t index) const
{
    return m_current_node->get(index);
}

/// helper functions:
bool MenuModel::has_back() const
{
    return !is_root() && m_back_navigation;
}

bool MenuModel::valid(std::size_t index) const
{
    return 0 <= index && index < rows();
}
