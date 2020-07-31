//
//  This program demonstrates the interaction and configuration of a MenuView.
//  It also demonstrates the ability to dynamically update a MenuView.
//
//  To test:
//    Build the program with at least C++11.
//    Change the font path declaration below if necessary, to point to a valid font file.
//      std::string fontpath = "...";
//    Run the program and test the menu view by clicking on buttons on the left
//      or the menu on the right.
//      Output should appear wherever std::cout prints (no output in this demo).
//

#include "widgets/application.hpp"
#include "widgets/menumodel.hpp"
#include "widgets/menuview.hpp"
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


const std::string fontpath = "res/fonts/luximr.ttf";


static std::string generate_string(std::string const& pre, int depth, int index)
{
    switch (depth)
    {
    case 0:
        return pre + std::string(1, 'A' + index);
    case 1:
        return pre + std::to_string(index + 1);
    default:
        return pre + "." + std::to_string(index + 1);
    }
}


class DemoApplication : public Application
{
public:
    DemoApplication();

private:
    FontRef font;

    MenuModel menu_model;
    unsigned depth = 0;
    int index = -1;
    
private:
    void init_menu_view();
    void init_buttons();
    
    void update_index(int index = -1);
    void update();
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
    : Application({50, 50, 640, 480}, "ListView Simple Demo")
{
    background(Themes::BACKGROUND);
    font = add_font(fontpath, 16, Themes::SECONDARY);
    init_menu_view();
    init_buttons();
}

void DemoApplication::init_menu_view()
{
    auto title_text = new TextItem({0, 0, width() - 180, 60});
    title_text->text("MenuView Demo").font(font).align(ALIGN_CENTER);
    this->add_item(title_text);

    menu_model.back_navigation(true);
    {
        auto a = menu_model.add("A");
        a->add("A1");
        a->add("A2");
        a->add("A3");
    }
    {
        auto b = menu_model.add("B");
        {
            auto b1 = b->add("B1");
            b1->add("B1.1");
            b1->add("B1.2");
        }
        b->add("B2");
        b->add("B3");
    }
    {
        menu_model.add("C");
    }
    
    auto menu_view = new MenuView({width() - 180, 0, 180, height()});
    menu_view->item_font(font).item_height(30);
    menu_view->margins(Margins(10));
    menu_view->model(&menu_model);
    menu_view->on_index_clicked([this](int index)
    {
        if (index >= 0)
        {
            std::cout << "index " << index << " clicked";
            std::cout << " (" << menu_model.at(index) << ")" << std::endl;
            
            if (menu_model.at(index) == MenuModel::BACK)
            {
                depth -= 1;
                menu_model.go_to_parent();
            }
            else
            {
                depth += 1;
                menu_model.go_to_index(index);
            }
            update_index();
            update();
        }
    });
    this->add_item("menu-view", menu_view);
}

void DemoApplication::init_buttons()
{
    auto target_button = new TextButton({child("menu-view")->x()/2 - 100, 120, 200, 40});
    target_button->background(Themes::PRIMARY);
    target_button->font(font).text("");
    target_button->on_left_clicked([this](MouseEvent const& event)
    {
        update_index(index + 1);
        update();
    });
    
    auto in_button = new TextButton(*target_button);
    in_button->y(170).width(95);
    in_button->text("In");
    in_button->on_left_clicked([this](MouseEvent const& event)
    {
        if (menu_model.go_to_index(index))
        {
            depth += 1;
            update_index();
            update();
        }
    });
    
    auto out_button = new TextButton(*in_button);
    out_button->x(child("menu-view")->x()/2 + 5);
    out_button->text("Out");
    out_button->on_left_clicked([this](MouseEvent const& event)
    {
        if (menu_model.go_to_parent())
        {
            depth -= 1;
            update_index();
            update();
        }
    });
    
    auto add_button = new TextButton(*target_button);
    add_button->y(220);
    add_button->text("Add");
    add_button->on_left_clicked([this](MouseEvent const& event)
    {
        if (index == -1)
            menu_model.add(generate_string(menu_model.node()->text, depth, int(menu_model.node()->size())));
        else
            menu_model.node_at(index)->add(generate_string(menu_model.node_at(index)->text, depth+1, int(menu_model.node_at(index)->size())));
        update();
    });
    
    auto clear_button = new TextButton(*target_button);
    clear_button->y(270);
    clear_button->text("Clear");
    clear_button->on_left_clicked([this](MouseEvent const& event)
    {
        if (index == -1)
            menu_model.clear();
        else
            menu_model.node_at(index)->clear();
        update();
    });
    
    this->add_item("target-button", target_button);
    this->add_item("in-button", in_button);
    this->add_item("out-button", out_button);
    this->add_item("add-button", add_button);
    this->add_item("clear-button", clear_button);
    
    update();
}

void DemoApplication::update_index(int index)
{
    if (0 <= index && index < menu_model.node()->size())
        this->index = index;
    else
        //  if index out of range, set to -1 (i.e. target the current node)
        this->index = -1;
}

void DemoApplication::update()

{
    //  update enable/disable state where needed
    if (index == -1 || menu_model.node()->is_final())
        child("in-button")->disable();
    else
        child("in-button")->enable();
    
    if (menu_model.root() == menu_model.node())
        child("out-button")->disable();
    else
        child("out-button")->enable();
    
    if (menu_model.node()->size() == 0)
        child("clear-button")->disable();
    else
        child("clear-button")->enable();
    
    //  update target text
    auto target_button = child<TextButton>("target-button");
    if (index == -1)
    {
        if (menu_model.root() == menu_model.node())
            target_button->text("Target: Main");
        else
            target_button->text("Target: " + menu_model.node()->text);
    }
    else
    {
        target_button->text("Target: " + menu_model.at(index));
    }
}