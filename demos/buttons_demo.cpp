//
//  Mouse and click around!
//   * Click, press, release, and hover on the upper buttons.
//   * Hover only works when no buttons are pressed.
//   * Click on the button labelled "click to move" several times. This tests move construction and assignment.
//   * Click on the button labelled "click to copy" several times and interact with the button on its right.
//      This tests copy assignment.
//

#include "widgets/application.hpp"
#include "widgets/textitem.hpp"
#include "widgets/textbutton.hpp"

#include "themes.hpp"

#include <iostream>
#include <string>
#include <vector>


const std::string fontpath = "demos/fonts/luxisr.ttf";


class DemoApplication : public Application
{
public:
    DemoApplication();

private:
    FontRef header_font;
    FontRef normal_font;
    
    bool moved = false;
    
private:
    void init_buttons();
};


int main(int argc, const char * argv[])
{
    try
    {
        DemoApplication app;
        return app.run();
    }
    catch (std::runtime_error& err)
    {
        std::cout << "An exception occurred..." << std::endl;
        std::cout << err.what() << std::endl;
    }
}


DemoApplication::DemoApplication()
    : Application({50, 50, 640, 480}, "Buttons Demo")
{
    header_font = add_font(fontpath, 18, Themes::SECONDARY);
    normal_font = add_font(fontpath, 14, Themes::SECONDARY);
    
    init_buttons();
}
    
void DemoApplication::init_buttons()
{
    auto title_text = new TextItem({0, 0, width(), 60}, this);
    title_text->text("Buttons Demo", header_font, ALIGN_CENTER);

    std::string texts[] = {
        "clicked", "left clicked", "right clicked",
        "pressed", "hovered",
    };

    std::vector<TextButton*> buttons(5, nullptr);
    for (auto i = 0; i < 5; ++i)
    {
        int col = i%3, row = i/3;
        buttons[i] = new TextButton({30+200*col, 120+100*row, 180, 80}, this);
        buttons[i]->background(Themes::PRIMARY);
        buttons[i]->text(texts[i], normal_font);
    }

    buttons[0]->on_clicked([](MouseEvent const&) { std::cout << "button clicked" << std::endl; });
    buttons[1]->on_left_clicked([](MouseEvent const&) { std::cout << "button left clicked" << std::endl; });
    buttons[2]->on_right_clicked([](MouseEvent const&) { std::cout << "button right clicked" << std::endl; });
    buttons[3]->on_pressed([](MouseEvent const&) { std::cout << "button pressed" << std::endl; });
    buttons[4]->on_hovered([](MouseEvent const&) { std::cout << "button hovered" << std::endl; });
}
