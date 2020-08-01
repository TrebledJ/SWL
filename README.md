# SWL

Simple Widgets Layer: Modern C++11 Widgets Wrapper for SDL2

## Setup and Installation
Required Frameworks and Libraries:

* [SDL2](https://www.libsdl.org/)
* [SDL2-Image](https://www.libsdl.org/projects/SDL_image/)
* [SDL2-Mixer](https://www.libsdl.org/projects/SDL_mixer/)
* [SDL2-TTF](https://www.libsdl.org/projects/SDL_ttf/)
* [SDL-FontCache](https://github.com/grimfang4/SDL_FontCache)

Copy the `src/` files from this repository into your project and enjoy. :)

## Concepts
* **RAII**: Wrap common SDL2 resource types and take advantage of C++ resource management.
* **Seperation of Concerns**: Event-handling and rendering are kept separate. Ideally, rendering does not modify state but only reads from it.
* **Initialisation**: Define widget properties and behaviour in the same place where widgets are created.

This library is by no means complete and only covers a subset of SDL2 functions.

## Hello World

```cpp
#include "widgets/application.hpp"  //  Application
#include "widgets/textbutton.hpp"   //  TextButton
#include "types.hpp"                //  FontRef
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
    
    return app.run();  //  "button" is automatically deleted
}
```
<sup>(demos/hello_world.cpp)</sup>

## Classes

The following types are provided:

### Widgets
* Application
* BaseApplication
* Button
* Canvas
* DataModel
* DataView
* Image
* ListModel
* ListView
* MenuModel
* MenuView
* RectItem
* StateMachine
* TextButton
* TextItem
* WidgetItem

### Utility

#### SDL-Wrapper Types
* Surface (wrapper for SDL_Surface)
* Texture (wrapper for SDL_Surface)
* Renderer (wrapper for SDL_Surface)
* Window (wrapper for SDL_Surface)
* TTFont (wrapper for SDL_Surface)
* SharedFont (strong-pointer wrapper for FC_Font)
* FontRef (weak-pointer wrapper for FC_Font)
* SharedMusic (strong-pointer wrapper for Mix_Music)
* MusicRef (weak-pointer wrapper for Mix_Music)

#### Convenience Types
* Point
* Size
* Margins (a.k.a. Padding)

#### Event Types
* MouseEvent
* WheelEvent
* KeyEvent

#### Others
* SpriteCache
* StateMachine
* Alignment (enum)

