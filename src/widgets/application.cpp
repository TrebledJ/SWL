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

#include "application.hpp"
#include "themes.hpp"


static constexpr unsigned FPS = 50;
static constexpr unsigned MUSIC_FADE_TIME_MS = 500;

/// constructor:
Application::Application(SDL_Rect const& dimensions, std::string const& window_title, Uint32 window_flags, Uint32 renderer_flags)
    : BaseApplication(dimensions.w, dimensions.h)
    , running{true}
    , music_enabled{true}
    , window_title{window_title}
    , window_flags{window_flags}
    , renderer_flags{renderer_flags}
{
    //  this gets run when scenes are changed
    scene_handler.set_update_action([this]()
                                    {
                                        set_active_music(MusicRef());
                                        foreach_child(&WidgetItem::hide);
                                    });
    
    background(Themes::BACKGROUND);
    
    Util::assert_true((window = make_window(window_title, x(), y(), width(), height(), window_flags)),
                      "[ERROR] Failed to initialise Window: ${sdl_error}");
    Util::assert_true((renderer = make_renderer(window, -1, renderer_flags)),
                      "[ERROR] Failed to initialise Renderer: ${sdl_error})");
    Util::assert_equals(SDL_RenderSetLogicalSize(renderer.get(), width(), height()), 0,
                        "[ERROR] Failed to set render logical size: ${sdl_error}");
}

/// destructor:
Application::~Application() = default;

/// modifiers:
int Application::run()
{
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
            handle_event(event);
        
        if (!running)
            break;
        
        loop();
        render();
    }
    return 0;
}

/// accessors:
Renderer const& Application::get_renderer() const
{
    return renderer;
}

/// protected modifiers:
FontRef Application::add_font(std::string const& filename, Uint32 point_size, SDL_Color const& color, int style)
{
    if (!renderer)
        return FontRef();
        
    auto font = make_shared_font(renderer, filename, point_size, color, style);
    Util::assert_true(font, "[ERROR] Failed to initialise font: " + filename);
    fonts.push_back(font);
    return FontRef(font);
}

MusicRef Application::add_music(std::string const& filename)
{
    if (!renderer)
        return MusicRef();
    
    auto music_item = make_shared_music(filename);
    Util::assert_true(music_item, "[ERROR] Failed to load music: " + filename);
    music.push_back(music_item);
    return MusicRef(music_item);
}

void Application::loop()
{
    scene_handler.update_state();
    redraw();
    update(renderer);
    
    if (active_music_changed)
    {
        if (!active_music.expired() && music_enabled)
            Mix_FadeInMusic(active_music.lock().get(), 1, MUSIC_FADE_TIME_MS);
        else
            Mix_FadeOutMusic(MUSIC_FADE_TIME_MS);
        
        active_music_changed = false;
    }
}

void Application::quit()
{
    running = false;
}

void Application::set_active_music(MusicRef const& music_ref)
{
    active_music = music_ref;
    active_music_changed = true;
}

void Application::toggle_music()
{
    music_enabled = !music_enabled;
    if (music_enabled)
    {
        if (!active_music.expired())
            Mix_FadeInMusic(active_music.lock().get(), 1, MUSIC_FADE_TIME_MS);
    }
    else
    {
        Mix_FadeOutMusic(MUSIC_FADE_TIME_MS);
    }
}

/// GUI events:
bool Application::handle_event(SDL_Event const& event)
{
    switch (event.type)
    {
    case SDL_QUIT:
        quit();
        break;
        
    //  delegate events to children
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEMOTION:
        handle_mouse_event(Util::make_mouse_event(event));
        break;
        
    case SDL_MOUSEWHEEL:
        handle_wheel_event(Util::make_wheel_event(event));
        break;
        
    case SDL_KEYUP:
    case SDL_KEYDOWN:
        handle_key_event(Util::make_key_event(event));
        break;
        
    default:
        break;
    }
    return true;
}

void Application::render() const
{
    //  clear
    reset_target(renderer);
    set_render_color(renderer, m_background_color);
    SDL_RenderClear(renderer.get());
    
    //  draw
    foreach_child([this](WidgetItem* item){ item->render(renderer); });
    
    //  show
    reset_target(renderer);
    SDL_RenderPresent(renderer.get());
    SDL_Delay(FPS); //  TODO: abstract the delay (framerate?)?
}
