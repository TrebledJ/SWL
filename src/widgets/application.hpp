/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
 */

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "baseapplication.hpp"
#include "canvas.hpp"
#include "themes.hpp"
#include "types.hpp"
#include "utility.hpp"

#include "statemachine.hpp"

#include "sdl_inc.hpp"
#include "sdl_ttf_inc.hpp"

#include <list>


//  TODO: allow decoupling of TTF, MIX modules with preprocessor commands?

/**
 * @brief   Extends BaseApplication with Window, Renderer, font, scene management,
 *          item event-handling and item rendering.
 *
 * @note    Fonts, music, and items added by their respective add() functions
 *          will be managed by Application.
 *
 * @note    When switching scenes:
 *           - the active_music is reset
 *           - all children of Application are hidden
 *          It is up to the derived Application class to include `add_scene_action(scene, ...)` that
 *          set any active music and show any necessary children.
 */
class Application : public BaseApplication
{
public:
    std::string window_title;
    Uint32 window_flags;
    Uint32 renderer_flags;
    
public:
    Application(SDL_Rect const& dimensions,
                std::string const& window_title = "",
                Uint32 window_flags = SDL_WINDOW_SHOWN,
                Uint32 renderer_flags = SDL_RENDERER_ACCELERATED);
    
    virtual ~Application();
    
    /// modifiers:
    /**
     * @brief   Executes the program
     */
    int run();
    
    /**
     * @brief   Creates a managed font
     * @return  Returns a reference to the font
     * @pre     Call after Application::init(). Renderer should be initialised,
     *          otherwise a null FontRef is returned.
     */
    FontRef add_font(std::string const& filename, Uint32 point_size, SDL_Color const& color, int style = TTF_STYLE_NORMAL);
    
    /**
     * @brief   Creates a managed music object, works similar to add_font()
     * @return  A reference to the music
     * @pre     Same pre-conditions as add_font()
     */
    MusicRef add_music(std::string const& filename);
    
    /**
     * @brief   Adds a scene to the application
     */
    template<class Enum>
    void add_scene_action(Enum scene, StateMachine::Action task = nullptr);

    template<class Enum>
    void set_scene(Enum scene);
    
    /// accessors:
    Renderer const& get_renderer() const;
    
    template<class Enum>
    Enum get_scene() const;

protected:
    bool running;
    bool music_enabled;
    
protected:
    virtual void loop();
    virtual void quit();
    
    /**
     * @brief   Sets the active music. This will be played the next time loop()
     *          is called (if music is enabled) or the next time music is enabled.
     * @note    Setting a null music (e.g. `MusicRef(nullptr)`) will not play any music.
     */
    void set_active_music(MusicRef const&);
    
    /// @brief  Toggles the current music (on / off)
    void toggle_music();
    
private:
    Window window;
    Renderer renderer;
    
    std::list<SharedFont> fonts;    //  manages fonts, deleting them at the end
    std::list<SharedMusic> music;   //  manages music
    
    MusicRef active_music;
    bool active_music_changed;      //  a flag checking whether the active music has changed
    
    StateMachine scene_handler;
    
private:
    /// GUI events:
    /**
     * @brief   Passes events to child items
     */
    bool handle_event(SDL_Event const& event);
    
    /**
     * @brief   Renders child items on the window
     */
    void render() const;
};
        
/// inline implementation:
template<class Enum>
inline void Application::add_scene_action(Enum scene, StateMachine::Action task)
{
    scene_handler.add_state_action(scene, task);
}

template<class Enum>
inline void Application::set_scene(Enum scene)
{
    scene_handler.set_next_state(scene);
}

template<class Enum>
inline Enum Application::get_scene() const
{
    return scene_handler.get_state<Enum>();
}

#endif
