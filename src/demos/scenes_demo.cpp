//
//  Click on these buttons to try out scenes!
//   * Clicking on "New" will bring you to a different "scene".
//   * Clicking on "Menu" will return you to the first scene.
//

#include "widgets/application.hpp"
#include "widgets/textitem.hpp"
#include "widgets/textbutton.hpp"
#include "widgets/canvas.hpp"

#include "themes.hpp"
#include "types.hpp"
#include "utility.hpp"

#include "sdl_ttf_inc.hpp"
#include "sdl_inc.hpp"

#include <iostream>
#include <string>
#include <vector>


const std::string fontpath = "fonts/luxisr.ttf";


enum class Scene
{
    MENU, MAIN
};


class DemoApplication : public Application
{
public:
    DemoApplication();
    
private:
    FontRef font;
    
private:
    void init_widgets();
    void init_menu();
    void init_main();
};


int main(int argc, const char* argv[])
{
    try
    {
        DemoApplication app;
        return app.run();
    } catch (std::runtime_error& err) {
        std::cout << "An exception occurred..." << std::endl;
        std::cout << err.what() << std::endl;
    }
}


DemoApplication::DemoApplication()
    : Application({50, 50, 640, 480}, "Scenes Demo")
{
    font = add_font(fontpath, 18, Themes::SECONDARY, TTF_STYLE_NORMAL);
    init_widgets();
    
    add_scene_action(Scene::MENU, [this](){
        child("title-text")->show();
        child("menu-scene")->show();
    });
    add_scene_action(Scene::MAIN, [this](){
        child("title-text")->show();
        child("main-scene")->show();
    });
    
    set_scene(Scene::MENU);
}

void DemoApplication::init_widgets()
{
    auto title_text = new TextItem({0, 0, width(), 60});
    title_text->text("Scenes Demo").font(font).align(ALIGN_CENTER);
    this->add_item("title-text", title_text);
    
    init_menu();
    init_main();
}

void DemoApplication::init_menu()
{
    auto menu_scene = new Canvas(width(), height(), get_renderer());
    menu_scene->background(Themes::BACKGROUND);
    this->add_canvas("menu-scene", menu_scene);
    
    constexpr int NUM_BUTTONS = 4;
    TextButton* buttons[NUM_BUTTONS];
    std::string text[NUM_BUTTONS] = { "New", "/", "/", "Exit" };
    for (int i = 0; i < NUM_BUTTONS; ++i)
    {
        buttons[i] = new TextButton;
        buttons[i]->pos(i * width()/NUM_BUTTONS + 1, height()-80).size(width()/NUM_BUTTONS - 2, 80);
        buttons[i]->text(text[i]).font(font);
        menu_scene->add_item(buttons[i]);
    }
    
    buttons[0]->on_clicked([this](MouseEvent const&)
    {
        set_scene(Scene::MAIN);
    });
    buttons[3]->on_clicked([this](MouseEvent const&)
    {
        SDL_Event event;
        event.type = SDL_QUIT;
        SDL_PushEvent(&event);
    });
}

void DemoApplication::init_main()
{
    auto main_scene = new Canvas(width(), height(), get_renderer());
    main_scene->background(Themes::BACKGROUND);
    this->add_canvas("main-scene", main_scene);
    
    constexpr int NUM_BUTTONS = 5;
    TextButton* buttons[NUM_BUTTONS];
    std::string text[NUM_BUTTONS] = { "Tab 1", "Tab 2", "Tab 3", "Tab 4" };
    for (int i = 0; i < NUM_BUTTONS-1; ++i)
    {
        buttons[i] = new TextButton;
        buttons[i]->pos(0, i*60).size(120, 60);
        buttons[i]->text(text[i]).font(font);
        buttons[i]->on_clicked([text, i](MouseEvent const&) { std::cout << text[i] << " clicked" << std::endl; });
        main_scene->add_item(buttons[i]);
    }
    buttons[4] = new TextButton;
    buttons[4]->pos(width() - 120, height() - 60).size(120, 60);
    buttons[4]->text("Menu").font(font);
    buttons[4]->on_clicked([this](MouseEvent const&)
    {
        set_scene(Scene::MENU);
    });
    main_scene->add_item(buttons[4]);
}
