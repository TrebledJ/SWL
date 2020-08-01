//
//  This program demonstrates the scalability of the ListModel and ListView along with
//  how to configure them (see init_listview()).
//
//  Scroll and click on the items inside the listview!
//   * Adjust the NUM_LIST_ITEMS constant below to a higher value (try 2000).
//

#include "widgets/application.hpp"
#include "widgets/listmodel.hpp"
#include "widgets/listview.hpp"
#include "widgets/textitem.hpp"

#include "themes.hpp"
#include "types.hpp"
#include "utility.hpp"

#include "sdl_ttf_inc.hpp"
#include "sdl_inc.hpp"

#include <iostream>
#include <string>
#include <vector>


const std::string fontpath = "fonts/luxisr.ttf";
const unsigned NUM_LIST_ITEMS = 50;


//  ListModel/ListView are designed for classes that inherit ListItem,
//  the core reason being that the fields() and fields_at() accessors are used.
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
};


class DemoApplication : public Application
{
public:
    DemoApplication();
    
private:
    FontRef header_font;
    FontRef normal_font;
    
    ListModel<Employee> employees;

private:
    void init_listview();
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
    header_font = add_font(fontpath, 18, Themes::SECONDARY);
    normal_font = add_font(fontpath, 14, Themes::SECONDARY);
    init_listview();
}
    
void DemoApplication::init_listview()
{
    auto title_text = new TextItem({0, 0, width(), 60});
    title_text->text("Employees").font(header_font).align(ALIGN_CENTER);
    add_item(title_text);
    
    //  here the initialiser lists are implicitly convert to the type used by
    //  the listview, saving the lives of many finger joints
    employees.add({1, "Root",     "Log",      "HR"})
                .add({2, "John",     "Clark",    "Specialist"})
                .add({3, "Mr.",      "Bean",     "Comedian"})
                .add({4, "Mother",   "Teresa",   "Mediator"})
                .add({5, "Michael",  "Jordan",   "Player"})
                .add({6, "Steve",    "Jobs",     "Ideas Person"})
                .add({7, "Mike",     "Wazowski", "Optimist"})
                .add({8, "Clifford", "Red",      "Child Support"})
                .add({9, "Homer",    "Simpson",  "Buffoon Dad"})
                .add({10,"Puss",     "in-Boots", "Kitty Cat"})
                .add({11,"Luke",     "Skywalker","Jedi Knight"})
                .add({12,"Hudson",   "Taylor",   "Evangelist"})
                .add({13,"Santa",    "Claws",    "Malformed Mascot"})
                .add({14,"Johann",   "S. Bach",  "Reknowned Baroque Organist"})
                .add({15,"Steve",    "Reich",    "Minimalist Composer"})
                .add({16,"Doctor",   "Doof",     "Cartoon Antagonist"})
                .add({17,"Pikachu",  "",         "Cute Lightning Mouse"})
                .add({18,"Tony",     "Stark",    "Fly Guy"})
                .add({19,"Captain",  "Hook",     "Cartoon Antagonist"});
    for (auto i = 21; i <= NUM_LIST_ITEMS; ++i)
    {
        employees.add({i, std::string(3, 'A'+i%26), std::string(3, 'a'+i%26), std::string(1, '0'+i%10)});
    }

    auto listview = new ListView<Employee>({20, 60, 600, 300});
    listview->headers({"ID", "First", "Last", "Job"}).header_font(header_font).header_height(30);
    listview->item_font(normal_font).item_height(20);
    listview->column_ratios({1, 2, 2, 4}).margins(Margins(10));
    listview->item_padding(Padding{0, 0, 3, 0});
    listview->model(&employees);
    listview->selection_color(Colors::LIGHT_BLUE);
    listview->on_index_clicked([this](int index)
    {
        if (index >= 0)
        {
            std::cout << "index " << index << " clicked" << std::endl;
            employees.toggle_select(index);
        }
    });
    add_item(listview);
}
