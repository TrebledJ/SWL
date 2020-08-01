/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#ifndef MENUMODEL_HPP
#define MENUMODEL_HPP

#include "datamodel.hpp"
#include <memory>
#include <string>
#include <vector>


/**
 * @brief   Represents a node in a menu, holding data and containing child nodes.
 * @note    Children are fully managed. Deleting the root node deletes all children.
 */
class MenuNode
{
public:
    std::string text;
    
public:
    MenuNode(std::string const& text = "");

    /// modifiers:
    /// @return The new node
    MenuNode* add(std::string const& text);
    void clear();
    
    /// accessors:
    MenuNode* get(std::size_t index) const;
    MenuNode* get(std::string const& text) const;
    MenuNode* parent() const;
    std::size_t size() const;
    
    /// @brief  Checks if the current node is a leaf node (childless)
    bool is_final() const;
    
private:
    MenuNode* m_parent; //  weak pointer
    std::vector<std::unique_ptr<MenuNode>> m_children;  //  children nodes are destroyed
};


/**
 * @brief   A model for a list of options. Includes an optional "Back" option.
 */
class MenuModel : public DataModel<std::string>
{
public:
    static const std::string BACK;
    
public:
    MenuModel() noexcept;
    
    /// modifiers:
    /// @brief  Adds an item to the current node
    MenuNode* add(std::string const& text);
    /// @brief  Clears all items of the current node
    void clear();
    /// @brief  Set whether a "Back" item should be added to the model as a 'shadow'  (only for non-root nodes)
    void back_navigation(bool on);
    
    //  use these to navigate the model
    void go_to_root();
    
    /// @return Whether the operation was successful
    bool go_to_parent();
    bool go_to_index(std::size_t index);
    bool go_to_option(std::string const& text);
    
    /// accessors:
    std::size_t rows() const;
    std::string const& at(std::size_t index) const;
    
    /// @brief  Checks if the node at `index` is final
    bool is_final(std::size_t index) const;
    bool is_root() const;
    
    /// @return The root node
    MenuNode* root() const;
    /// @return The current node
    MenuNode* node() const;
    MenuNode* node_at(std::size_t index) const;
    
private:
    std::unique_ptr<MenuNode> m_root_node;  //  when destructed, this will set off a "chain reaction",
                                            //  destructing child nodes, including the current node
    MenuNode* m_current_node;
    bool m_back_navigation;
    
private:
    /// helper functions:
    
    /// @return Whether this node should have a "Back" item (only for non-root nodes)
    bool has_back() const;
    /// @return Whether `index` is valid, i.e. can be queried with at()
    bool valid(std::size_t index) const;
};


#endif /* menumodel_hpp */
