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

#ifndef TYPES_HPP
#define TYPES_HPP

#include "SDL_FontCache.hpp"
#include "sdl_ttf_inc.hpp"
#include "sdl_mixer_inc.hpp"
#include "sdl_inc.hpp"

#include <functional>
#include <memory>
#include <string>


//
//  Types
//

/// SDL wrapper types:
template<class T>
using UPointer = std::unique_ptr<T, std::function<void(T*)>>;
using Surface = UPointer<SDL_Surface>;
using Texture = UPointer<SDL_Texture>;
using Renderer = UPointer<SDL_Renderer>;
using Window = UPointer<SDL_Window>;
using TTFont = UPointer<TTF_Font>;
using SharedFont = std::shared_ptr<FC_Font>;
using FontRef = std::weak_ptr<FC_Font>;
using SharedMusic = std::shared_ptr<Mix_Music>;
using MusicRef = std::weak_ptr<Mix_Music>;


/// convenience types:
struct ValueRange {
    int min;
    int max;
    
    /// @brief  Checks if a value falls within the ValueRange (inclusive on both ends)
    bool contains(int value) const;
};

struct Point
{
    int x;
    int y;
    Point();
    Point(int x, int y);
    
    bool operator== (Point const&) const;
    bool operator!= (Point const&) const;
    
    /**
     * @return  Manhattan distance between two points
     */
    unsigned distance(Point const&) const;
    static unsigned distance(Point const&, Point const&);
    
    /**
     * @brief   Checks if given point is adjacent to this point
     */
    bool is_adjacent(Point const&) const;
    
    friend std::string to_string(Point const&);
};

struct Size
{
    int w;
    int h;
    
    Size();
    Size(int w, int h);
};

struct Margins
{
    int top;
    int bottom;
    int left;
    int right;
    
    /**
     * @brief   Sets all margins to `n`
     */
    Margins(int n = 0);
    
    /**
     * @brief   Sets individual margins
     */
    Margins(int top, int bottom, int left, int right);
};

using Padding = Margins;    //  same members, just a different type


/// event types:
/**
 * @brief   A simplified event struct encapsulating mouse button/movement events
 * @note    Does NOT include SDL_MOUSEWHEELEVENT. Use WheelEvent instead.
 */
struct MouseEvent
{
    enum Type
    {
        DOWN = SDL_MOUSEBUTTONDOWN,
        UP = SDL_MOUSEBUTTONUP,
        MOTION = SDL_MOUSEMOTION,
    };
    Type type;
    Uint32 button;
    Point pos;
    
    MouseEvent(SDL_MouseButtonEvent const& event, Point const& offset = Point());
    MouseEvent(SDL_MouseMotionEvent const& event, Point const& offset = Point());
    MouseEvent offset(int x, int y) const;
    MouseEvent offset(Point const& point) const;
};

/**
 * @brief   A simplified event struct encapsulating mouse wheel events
 *
 * This class is provided to deal with offset issues.
 * SDL_MouseWheelEvent doesn't have an xy pos to reference.
 */
struct WheelEvent
{
    Point wheel;
    Point pos;
    
    WheelEvent(SDL_MouseWheelEvent const& event, Point const& offset = Point());
    WheelEvent offset(int x, int y) const;
    WheelEvent offset(Point const& point) const;
};

using KeyEvent = SDL_KeyboardEvent; //  temporary, KeyEvent may change to a whole class in the future


//
//  Implementation
//

/// ValueRange:
inline bool ValueRange::contains(int value) const { return min <= value && value <= max; }

/// Point:
inline Point::Point() : Point(0, 0) {}
inline Point::Point(int x, int y) : x{x}, y{y} {}
inline bool Point::operator== (Point const& rhs) const { return x == rhs.x && y == rhs.y; }
inline bool Point::operator!= (Point const& rhs) const { return x != rhs.x || y != rhs.y; }
inline bool Point::is_adjacent(Point const& other) const { return distance(other) == 1; }
inline unsigned Point::distance(Point const& other) const { return std::abs(other.x - x) + std::abs(other.y - y); }
inline unsigned Point::distance(Point const& a, Point const& b) { return a.distance(b); }
inline std::string to_string(Point const& pos) { return "(" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")"; }

/// Size:
inline Size::Size() : Size(0, 0) {}
inline Size::Size(int w, int h) : w{w}, h{h} {}

/// Margins:
inline Margins::Margins(int n) : Margins(n, n, n, n) {}
inline Margins::Margins(int top, int bottom, int left, int right)
    : top{top}, bottom{bottom}, left{left}, right{right}
{
}

/// MouseEvent:
inline MouseEvent::MouseEvent(SDL_MouseButtonEvent const& event, Point const& offset)
    : type{static_cast<Type>(event.type)}
    , button{event.button}
    , pos{event.x - offset.x, event.y - offset.y}
{
}
inline MouseEvent::MouseEvent(SDL_MouseMotionEvent const& event, Point const& offset)
    : type{MOTION}
    , button{event.state}
    , pos{event.x - offset.x, event.y - offset.y}
{
}

inline MouseEvent MouseEvent::offset(int x, int y) const
{
    auto copy = *this;
    copy.pos.x -= x;
    copy.pos.y -= y;
    return copy;
}

inline MouseEvent MouseEvent::offset(Point const& point) const
{
    return offset(point.x, point.y);
}

/// WheelEvent:
inline WheelEvent::WheelEvent(SDL_MouseWheelEvent const& event, Point const& offset)
    : wheel{event.x, event.y}
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    pos.x = x - offset.x;
    pos.y = y - offset.y;
}

inline WheelEvent WheelEvent::offset(int x, int y) const
{
    auto copy = *this;
    copy.pos.x -= x;
    copy.pos.y -= y;
    return copy;
}

inline WheelEvent WheelEvent::offset(Point const& point) const
{
    return offset(point.x, point.y);
}


#endif
