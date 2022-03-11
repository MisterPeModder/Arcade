#include <iostream>

#include <arcade/IDisplay.hpp>
#include <arcade/IGame.hpp>

#include "core.hpp"

extern "C"
{
#include <dlfcn.h>
}

namespace arcade
{
    class DummyGame : public IGame {
    };

    int arcade(std::span<std::string> args)
    {
        std::cout << "starting arcade...\n";
        std::cout << "args: \n";

        for (auto arg : args)
            std::cout << "- " << arg << '\n';
        std::cout.flush();

        if (args.size() != 3) {
            std::cerr << args[0] << ": expected two arguments" << std::endl;
            return 84;
        }

        void *displayLib(dlopen(args[1].c_str(), RTLD_NOW));

        if (displayLib == nullptr) {
            std::cerr << args[0] << ": " << dlerror() << std::endl;
            return 84;
        }

        void *gameLib(dlopen(args[2].c_str(), RTLD_NOW));

        if (gameLib == nullptr) {
            std::cerr << args[0] << ": " << dlerror() << std::endl;
            dlclose(displayLib);
            return 84;
        }

        IDisplay *(*displayEntry)()(reinterpret_cast<IDisplay *(*)()>(dlsym(displayLib, "arcade_DisplayEntryPoint")));

        if (displayEntry == nullptr) {
            std::cerr << args[0] << ": " << dlerror() << std::endl;
            dlclose(gameLib);
            dlclose(displayLib);
            return 84;
        }

        IGame *(*gameEntry)()(reinterpret_cast<IGame *(*)()>(dlsym(gameLib, "arcade_GameEntryPoint")));

        if (gameEntry == nullptr) {
            std::cerr << args[0] << ": " << dlerror() << std::endl;
            dlclose(gameLib);
            dlclose(displayLib);
            return 84;
        }

        IDisplay *display((*displayEntry)());
        IGame *game((*gameEntry)());

        display->setup();

        game->setup(*display);
        game->setState(IGame::State::Running);
        game->update(42.21);
        game->close();

        display->close();

        dlclose(gameLib);
        dlclose(displayLib);
        return 0;
    }
} // namespace arcade
