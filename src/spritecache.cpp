/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
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
