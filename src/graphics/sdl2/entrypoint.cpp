#include <iostream>

#include <arcade/IDisplay.hpp>

#include "Sdl2Display.hpp"

namespace arcade
{
    static IDisplay *DISPLAY_INSTANCE = nullptr;

    ARCADE_DISPLAY_ENTRY_POINT
    {
        std::cout << "[sdl2]: called entry point" << std::endl;
        return DISPLAY_INSTANCE;
    }

    [[gnu::constructor]] void onConstruct()
    {
        DISPLAY_INSTANCE = new Sdl2Display();
        std::cout << "[sdl2]: constructed" << std::endl;
    }

    [[gnu::destructor]] void onDestroy()
    {
        delete DISPLAY_INSTANCE;
        DISPLAY_INSTANCE = nullptr;
        std::cout << "[sdl2]: destroyed" << std::endl;
    }
} // namespace arcade
