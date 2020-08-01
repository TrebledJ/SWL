//
//  This program demonstrates how the ListModel and ListView objects interact dynamically
//  with items. Buttons are used to interact with the ListModel.
//
//  Hire and fire employees by selecting them and clicking the corresponding buttons!
//   * Select employees on the left and click hire to move them to the right.
//   * Do the same with employees on the right (except you get to enjoy firing them).
//   * Employees are ordered by their ID.
//

#include "widgets/application.hpp"
#include "widgets/listmodel.hpp"
#include "widgets/listview.hpp"
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


struct Employee : public ListItem
{
    int id;
    std::string first_name;
    std::string last_name;
    std::string role;
    
    Employee(int id, std::string const& first_name, std::string const& last_name, std::string const& role)
        : id{id}, first_name{first_name}, last_name{last_name}, role{role}
    {
    }

    /// accessors:
    virtual std::size_t fields() const override { return 4; }
    virtual std::string field_at(int index) const override
    {
        switch (index)
        {
        case 0:     return std::to_string(id);
        case 1:     return first_name;
        case 2:     return last_name;
        case 3:     return role;
        default:    return "";
        }
    }
    
    /// convenience overload for debugging later
    friend std::ostream& operator<< (std::ostream& os, Employee const& e)
    {
        os << e.first_name << " " << e.last_name << " (" << e.role << ", id " << e.id << ")";
        return os;
    }
};


class DemoApplication : public Application
{
public:
    DemoApplication();
    
private:
    FontRef header_font;
    FontRef normal_font;
    
    ListModel<Employee> unemployed;
    ListModel<Employee> employed;
    
private:
    void init_widgets();
    void init_listmodels();
    void init_listview();
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
	: Application({50, 50, 640, 480}, "ListView Hire-and-Fire Demo")
{
    header_font = add_font(fontpath, 14, Themes::SECONDARY);
    normal_font = add_font(fontpath, 12, Themes::SECONDARY);
    init_widgets();
}
    
void DemoApplication::init_widgets()
{
    auto left_title_text = new TextItem({0, 0, width()/2, 60});
    left_title_text->text("Unemployed").font(header_font).align(ALIGN_CENTER);
    add_item(left_title_text);
    
    auto right_title_text = new TextItem(*left_title_text);
    right_title_text->pos(width()/2, 0);
    right_title_text->text("Employed");
    add_item(right_title_text);
    
    init_listmodels();
    init_listview();
    init_buttons();
}

void DemoApplication::init_listmodels()
{
    //  here the initialiser lists are implicitly convert to the type used by
    //  the listview, saving the lives of many finger joints
    unemployed.add({1, "Root",     "Log",      "HR"})
                .add({2, "John",     "Clark",    "Specialist"})
                .add({3, "Mr.",      "Bean",     "Comedian"})
                .add({4, "Mother",   "Teresa",   "Mediator"})
                .add({5, "Michael",  "Jordan",   "Player"})
                .add({6, "Steve",    "Jobs",     "Ideas Person"})
                .add({7, "Mike",     "Wazowski", "Optimist"})
                .add({8, "Clifford", "Red Dog",  "Child Support"})
                .add({9, "Homer",    "Simpson",  "Buffoon Dad"})
                .add({10,"Puss",     "in-Boots", "Kitty Cat"})
                .add({11,"Luke",     "Skywalker","Jedi Knight"})
                .add({12,"Hudson",   "Taylor",   "Evangelist"})
                .add({13,"Santa",    "Claws",    "Mascot"})
                .add({14,"Johann",   "S. Bach",  "Composer"})
                .add({15,"Steve",    "Reich",    "Composer"})
                .add({16,"Doctor",   "Doof",     "Antagonist"})
                .add({17,"Pikachu",  "",         "Mouse"})
                .add({18,"Tony",     "Stark",    "Fly Guy"})
                .add({19,"Captain",  "Hook",     "Antagonist"});
    
    unemployed.sorter([](Employee const& a, Employee const& b) { return a.id < b.id; });
    employed.sorter([](Employee const& a, Employee const& b) { return a.id < b.id; });
}
    
void DemoApplication::init_listview()
{
    auto hireable = new ListView<Employee>({20, 60, 290, 320});
    hireable->headers({"ID", "First", "Last", "Job"}).header_font(header_font).header_height(30);
    hireable->item_font(normal_font).item_height(20);
    hireable->column_ratios({1, 3, 3, 4}).margins(Margins(10));
    hireable->item_padding(Padding{0, 0, 3, 0});
    hireable->model(&unemployed);
    hireable->selection_color(Colors::LIGHT_BLUE);
    hireable->on_index_clicked([this](int index)
    {
        if (index != -1)
            unemployed.toggle_select(index);
    });
    add_item(hireable);
    
    auto fireable = new ListView<Employee>({width() / 2 + 10, 60, 290, 320});
    fireable->headers({"ID", "First", "Last", "Job"}).header_font(header_font).header_height(30);
    fireable->item_font(normal_font).item_height(20);
    fireable->column_ratios({1, 3, 3, 4}).margins(Margins(10));
    fireable->item_padding(Padding{0, 0, 3, 0});
    fireable->model(&employed);
    fireable->selection_color(Colors::LIGHT_BLUE);
    fireable->on_index_clicked([this](int index)
    {
        if (index != -1)
            employed.toggle_select(index);
    });
    add_item(fireable);
}

void DemoApplication::init_buttons() {
    auto button_hire = new TextButton({20, 400, 290, 60});
    button_hire->text("Hire").font(header_font);
    add_item(button_hire);
    
    auto button_fire = new TextButton({width() / 2 + 10, 400, 290, 60});
    button_fire->text("Fire").font(header_font);
    add_item(button_fire);
    
    button_hire->on_clicked([this](MouseEvent const&)
    {
        auto items = unemployed.selected_items();
        for (auto& item : items)
        {
            std::cout << "hired employee " << item << std::endl;
            item.selected = false; //  deselect
        }
        employed.add_items(items);
        unemployed.remove_selected_items();
    });
    
    button_fire->on_clicked([this](MouseEvent const&)
    {
        auto items = employed.selected_items();
        for (auto& item : items)
        {
            std::cout << "fired employee " << item << std::endl;
            item.selected = false; //  deselect
        }
        unemployed.add_items(items);
        employed.remove_selected_items();
    });
}
