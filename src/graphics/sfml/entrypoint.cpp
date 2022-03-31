#include <iostream>

#include <arcade/IDisplay.hpp>

#include "SfmlDisplay.hpp"

namespace arcade
{
    static IDisplay *DISPLAY_INSTANCE = nullptr;

    ARCADE_DISPLAY_ENTRY_POINT
    {
        std::cout << "[sfml]: called entry point" << std::endl;
        return DISPLAY_INSTANCE;
    }

    [[gnu::constructor]] void onConstruct()
    {
        DISPLAY_INSTANCE = new SfmlDisplay();
        std::cout << "[sfml]: constructed" << std::endl;
    }

    [[gnu::destructor]] void onDestroy()
    {
        delete DISPLAY_INSTANCE;
        DISPLAY_INSTANCE = nullptr;
        std::cout << "[sfml]: destroyed" << std::endl;
    }
} // namespace arcade
