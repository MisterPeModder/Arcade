#include <iostream>
#include <span>
#include <string>

#include <arcade/IDisplay.hpp>
#include <arcade/IGame.hpp>

#include "core.hpp"
#include "util/DynamicLibrary.hpp"

namespace arcade
{
    int arcade(std::span<std::string> args)
    {
        if (args.size() != 3) {
            std::cerr << args[0] << ": expected two arguments" << std::endl;
            return 84;
        }

        DynamicLibrary displayLib(args[1]);
        DynamicLibrary gameLib(args[2]);

        IDisplay *display(displayLib.symbol<IDisplay::EntryPoint>(IDisplay::ENTRY_POINT)());
        IGame *game(gameLib.symbol<IGame::EntryPoint>(IGame::ENTRY_POINT)());

        display->setup();

        game->setup(*display);
        game->setState(IGame::State::Running);
        game->update(42.21);
        game->close();

        display->close();

        return 0;
    }
} // namespace arcade
