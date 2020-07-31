/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
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
