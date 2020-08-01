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
#include "types.hpp"
#include "utility.hpp"

#include "sdl_ttf_inc.hpp"
#include "sdl_inc.hpp"

#include <iostream>
#include <string>
#include <vector>


const std::string fontpath = "fonts/luxisr.ttf";


class DemoApplication : public Application
{
public:
    DemoApplication();

private:
    FontRef header_font;
    FontRef normal_font;
    
    int ind;
    int ind_copy;
    
private:
    void init_buttons();
};


int main(int argc, const char * argv[])
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
    : Application({50, 50, 640, 480}, "Buttons Demo")
{
    header_font = add_font(fontpath, 18, Themes::SECONDARY);
    normal_font = add_font(fontpath, 14, Themes::SECONDARY);
    
    init_buttons();
}
    
void DemoApplication::init_buttons()
{
    auto title_text = new TextItem({0, 0, width(), 60});
    title_text->text("Buttons Demo").font(header_font).align(ALIGN_CENTER);
    add_item(title_text);

    std::string texts[] = {
        "clicked", "left clicked", "right clicked",
        "pressed", "hovered",
    };
    
    std::vector<TextButton*> buttons(5, nullptr);
    for (auto i = 0; i < 5; ++i)
    {
        int col = i%3, row = i/3;
        buttons[i] = new TextButton({30+200*col, 60+100*row, 180, 80});
        buttons[i]->background(Themes::PRIMARY);
        buttons[i]->text(texts[i]).font(normal_font);
        
        add_item(buttons[i]);
    }
    
    buttons[0]->on_clicked([](MouseEvent const&) { std::cout << "button clicked" << std::endl; });
    buttons[1]->on_left_clicked([](MouseEvent const&) { std::cout << "button left clicked" << std::endl; });
    buttons[2]->on_right_clicked([](MouseEvent const&) { std::cout << "button right clicked" << std::endl; });
    buttons[3]->on_pressed([](MouseEvent const&) { std::cout << "button pressed" << std::endl; });
    buttons[4]->on_hovered([](MouseEvent const&) { std::cout << "button hovered" << std::endl; });

    ind = 1;
    auto button_mover = new TextButton({30, 60+100*2, 180, 80});
    button_mover->background(Themes::PRIMARY);
    button_mover->text("click to move").font(normal_font);

    auto button_moving1 = new TextButton({30+200, 60+100*2, 180, 80});
    button_moving1->background(Themes::PRIMARY);
    button_moving1->text("mover").font(normal_font);

    auto button_moving2 = new TextButton;
    button_moving2->hide();
    
    button_mover->on_clicked([this, button_moving1, button_moving2](MouseEvent const&)
    {
        std::cout << "mover button clicked" << std::endl;
        if (ind == 1)
        {
            std::cout << "moving mover 1: (mov-ctor + cpy-asgn)" << std::endl;
            TextButton tmp = std::move(*button_moving1);
            *button_moving2 = tmp;
            button_moving2->pos(30+400, 60+100*2).show();
            button_moving1->hide();
            ind = 2;
        }
        else
        {
            std::cout << "moving mover 2: (mov-asgn)" << std::endl;
            *button_moving1 = std::move(*button_moving2);
            button_moving1->pos(30+200, 60+100*2).show();
            button_moving2->hide();
            ind = 1;
        }
        std::cout << "done" << std::endl;
    });
    
    add_item(button_mover);
    add_item(button_moving1);
    add_item(button_moving2);
    

    ind_copy = 0;
    auto button_copier = new TextButton({30, 60+100*3, 180, 80});
    button_copier->background(Themes::PRIMARY);
    button_copier->text("click to copy").font(normal_font);

    auto button_copy_here = new TextButton;
    button_copy_here->hide();

    button_copier->on_clicked([this, button_copy_here, buttons](MouseEvent const&)
    {
        std::cout << "copier button clicked" << std::endl;
        *button_copy_here = *buttons[ind_copy];
        button_copy_here->pos(30+200, 60+100*3);

        std::cout << "copied button " << ind_copy << std::endl;
        ind_copy = (ind_copy + 1) % 5;
    });
    
    add_item(button_copier);
    add_item(button_copy_here);
}
