#include "widgets/application.hpp"  //  Application
#include "widgets/textbutton.hpp"   //  TextButton
#include "types.hpp"
#include "themes.hpp"               //  Themes
#include <iostream>


int main()
{
    Application app({50, 50, 640, 480}, "Hello World");
    auto font = app.add_font("demos/fonts/luxisr.ttf", 12, Themes::SECONDARY);
    auto button = new TextButton({app.width() / 2 - 60, app.height() / 2 - 40, 120, 80});
    app.add_item(button);
    
    button->text("Hello World!").font(font);
    button->on_clicked([](MouseEvent const&) { std::cout << "button clicked!" << std::endl; });
    
    return app.run();
}
