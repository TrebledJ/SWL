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

#ifndef SPRITECACHE_HPP
#define SPRITECACHE_HPP

#include "types.hpp"
#include <string>
#include <unordered_map>

/**
 * @brief   A singleton-esque class for managing sprites
 *
 * Essentially, this will stock up your nearest convenience store with all
 * the Sprites you'll ever need to bring to your parties. ^_^
 */
class SpriteCache
{
public:
    /// @brief  Loads a sprite from key
    static void load(std::string const& key);
    
    /// @brief  Loads a sprite from filename and associates it with key
    static void load(std::string const& key, std::string const& filename);
    
    /**
     * @brief   Retrieves the sprite associated with key.
     *          Sprite is loaded if the key is not found.
     */
    static const Surface& get(std::string const& key);
    
    /// @brief  Check if a given key exists
    static bool has(std::string const& key);
    
private:
    static std::unordered_map<std::string, Surface> m_cache_map;
};


/**
 * @brief   References a sprite object
 */
class SpriteRef
{
public:
    SpriteRef(const char* key);
    SpriteRef(std::string const& key);
    
    /// accessors:
    const Surface& get() const;
    
    operator bool() const;
    bool operator! () const;
    
private:
    std::string key;    //  used to query SpriteCache
};



#endif
