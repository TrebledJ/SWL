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

#include "spritecache.hpp"
#include "utility.hpp"
#include "sdl_image_inc.hpp"


//
//  SpriteCache
//

std::unordered_map<std::string, Surface> SpriteCache::m_cache_map;

/// @brief  Loads a sprite from key
void SpriteCache::load(std::string const& key)
{
    load(key, key);
}

/// @brief  Loads a sprite from filename and associates it with key
void SpriteCache::load(std::string const& key, std::string const& filename)
{
    m_cache_map[key] = make_surface(IMG_Load(filename.data()));
}

/**
 * @brief   Retrieves the sprite associated with key.
 *          Sprite is loaded if the key is not found.
 */
const Surface& SpriteCache::get(std::string const& key)
{
    auto it = m_cache_map.find(key);
    if (it != m_cache_map.end())
        return it->second;
    
    //  attempt to load the sprite
    load(key);
    return m_cache_map[key];
}

bool SpriteCache::has(std::string const& key)
{
    return m_cache_map.find(key) != m_cache_map.end();
}


//
//  SpriteRef
//

SpriteRef::SpriteRef(const char* key)
    : key{key}
{
}

SpriteRef::SpriteRef(std::string const& key)
    : key{key}
{
}

/// accessors:
const Surface& SpriteRef::get() const
{
    return SpriteCache::get(key);
}

SpriteRef::operator bool() const
{
    return SpriteCache::has(key);
}

bool SpriteRef::operator! () const
{
    return !SpriteCache::has(key);
}
