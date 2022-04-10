#include "Nibbler.hpp"

namespace arcade
{
    static arcade::IGame *GAME_INSTANCE = nullptr;

    ARCADE_GAME_ENTRY_POINT
    {
        std::cout << "[nibbler]: called entry point" << std::endl;
        return GAME_INSTANCE;
    }

    [[gnu::constructor]] void onConstruct()
    {
        GAME_INSTANCE = new Nibbler();
        std::cout << "[nibbler]: constructed" << std::endl;
    }

    [[gnu::destructor]] void onDestroy()
    {
        delete GAME_INSTANCE;
        GAME_INSTANCE = nullptr;
        std::cout << "[nibbler]: destroyed" << std::endl;
    }
} // namespace arcade
