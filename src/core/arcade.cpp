#include <iostream>
#include <span>
#include <string>

#include <arcade/IDisplay.hpp>

#include "Core.hpp"
#include "arcade.hpp"
#include "util/DynamicLibrary.hpp"

namespace arcade
{
    int arcade(std::span<std::string> args)
    {
        if (args.size() != 2) {
            std::cerr << args[0] << ": expected graphics backend argument" << std::endl;
            return 84;
        }

        DynamicLibrary::Registry libs;

        DynamicLibrary &defaultGraphicsLib(DynamicLibrary::load(args[1], libs));
        IDisplay *defaultGraphics(defaultGraphicsLib.symbol<IDisplay::EntryPoint>(IDisplay::ENTRY_POINT)());

        DynamicLibrary::loadDirectory("./lib", libs);

        {
            Core core(libs, defaultGraphics);

            core.eventLoop();
        }

        return 0;
    }
} // namespace arcade
