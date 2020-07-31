//
//  To test:
//    Build the program with at least C++11.
//    Change the font path declaration below if necessary, to point to a valid font file.
//      std::string fontpath = "...";
//    Run the program and test the canvases by interacting with the items inside.
//      Output should appear wherever std::cout prints.
//    "A" buttons respond when clicked. "B" buttons respond when pressed.
//    "C" buttons respond when hovered. The ListView in canvas 3 should respond when scrolled.
//

#include "widgets/application.hpp"
#include "widgets/textitem.hpp"
#include "widgets/textbutton.hpp"
#include "widgets/canvas.hpp"
#include "widgets/listmodel.hpp"
#include "widgets/listview.hpp"

#include "themes.hpp"
#include "types.hpp"
#include "utility.hpp"

#include "sdl_ttf_inc.hpp"
#include "sdl_inc.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>


const std::string fontpath = "res/fonts/luximr.ttf";


enum UnitClass
{
    Warrior, Archer, Wizard, Ninja, Medic, Berserker, Ranger,
    NUM_UNIT_CLASSES
};

std::string unit_class_to_str(UnitClass uc)
{
    switch (uc)
    {
    case UnitClass::Warrior:    return "Warrior";
    case UnitClass::Archer:     return "Archer";
    case UnitClass::Wizard:     return "Wizard";
    case UnitClass::Ninja:      return "Ninja";
    case UnitClass::Medic:      return "Medic";
    case UnitClass::Berserker:  return "Berserker";
    case UnitClass::Ranger:     return "Ranger";
    default: return "";
    }
}

struct Unit : public ListItem
{
    UnitClass unit_class;
    int health;
    int strength;
    int armour;
    
    Unit(UnitClass uc, int h, int s, int a)
        : unit_class{uc}
        , health{h}
        , strength{s}
        , armour{a}
    {
    }
        
    virtual std::size_t fields() const override { return 4; }
    virtual std::string field_at(int index) const override
    {
        switch (index)
        {
        case 0:     return unit_class_to_str(unit_class);
        case 1:     return std::to_string(health);
        case 2:     return std::to_string(strength);
        case 3:     return std::to_string(armour);
        default:    return "";
        }
    }
};


class DemoApplication : public Application
{
public:
    DemoApplication();
    
protected:
    void init_widgets();
    
private:
    FontRef font_normal;
    FontRef font_small;
        
    ListModel<Unit> model;
};


int main(int argc, const char * argv[])
{
    DemoApplication app;
    return app.run();
}


DemoApplication::DemoApplication()
    : Application({50, 50, 640, 480}, "Canvas Demo")
{
    background(Themes::BACKGROUND);
    
    font_normal = add_font(fontpath, 18, Themes::SECONDARY);
    font_small = add_font(fontpath, 14, Themes::SECONDARY);
    
    init_widgets();
}
    
void DemoApplication::init_widgets()
{
    auto title_text = new TextItem({0, 0, width(), 60});
    title_text->text("Canvas Demo").font(font_normal).align(ALIGN_CENTER);
    this->add_item(title_text);
    
    Canvas* canvas[4];
    for (int i = 0; i < 4; ++i)
    {
        canvas[i] = new Canvas(width()/2 - 10, (height() - 60) / 2 - 10, get_renderer());
        canvas[i]->pos(5 + (width() / 2) * (i % 2), 65 + ((height()-60) / 2) * (i / 2));
        canvas[i]->background(Themes::PRIMARY);
        this->add_canvas(canvas[i]);
        
        //  note the position of the following items is RELATIVE to the canvas
        auto canvas_title = new TextItem;
        canvas_title->size(canvas[i]->width(), 30);
        canvas_title->text("Canvas " + std::to_string(i)).font(font_normal).align(ALIGN_CENTER);
        canvas[i]->add_item(canvas_title);
        
        if (i == 3) //  we'll save some goodies for i=3
            continue;
    
        //  make some buttons, because who doesn't like buttons?
        SDL_Color colors[3] = {Colors::RED, Colors::GREEN, Colors::ORANGE};
        for (int j = 0; j < 3; ++j)
        {
            auto button = new TextButton;
            char id = 'A' + j;
            button->pos(5 + canvas[i]->width() * j/3, 35)
                    .size(canvas[i]->width()/3 - 10, canvas[i]->height() - 40);
            button->background(colors[j]);
            button->text(std::string(1, id)).font(font_normal);
            if (j == 0) button->on_clicked([i, id](MouseEvent const&) { std::cout << "canvas " << i << ": button " << id << " clicked" << std::endl; });
            if (j == 1) button->on_pressed([i, id](MouseEvent const&) { std::cout << "canvas " << i << ": button " << id << " pressed" << std::endl; });
            if (j == 2) button->on_hovered([i, id](MouseEvent const&) { std::cout << "canvas " << i << ": button " << id << " hovered" << std::endl; });
            canvas[i]->add_item(button);
        }
    }
    
    //  init ListModel/View
    for (int i = 0; i < 30; ++i)
    {
        model.add(Unit{
            static_cast<UnitClass>((i * 97) % NUM_UNIT_CLASSES),
            ((i+1)*10007) % 32 + 5, //  care for some simple PRNG?
            ((i+1)*8009) % 16 + 5,
            ((i+1)*12713) % 64 + 5,
        });
    }
    
    auto lview = new ListView<Unit>({5, 35, canvas[3]->width() - 10, canvas[3]->height() - 40});
    lview->header_font(font_normal).item_font(font_small);
    lview->header_height(40).item_height(20);
    lview->headers({"Class", "HP", "Str", "Amr"}).column_ratios({2, 1, 1, 1});
    lview->model(&model);
    lview->selection_color(Colors::LIGHT_BLUE);
    lview->on_index_clicked([this, canvas](int index)
    {
        if (index != -1)
        {
            std::cout << "canvas 3: index " << index << " clicked" << std::endl;
            model.toggle_select(index);
            canvas[3]->redraw();
        }
    });
    lview->on_scrolled([canvas](WheelEvent const&) { canvas[3]->redraw(); });
    canvas[3]->add_item(lview);
}
