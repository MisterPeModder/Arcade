#include <iostream>
#include <span>
#include <stdexcept>
#include <string>

#include <arcade/IDisplay.hpp>

#include "Core.hpp"
#include "arcade.hpp"
#include "util/DynamicLibrary.hpp"

namespace arcade
{
    int arcade(std::span<std::string> args)
    {
        // Argument checking
        if (args.size() != 2)
            throw std::runtime_error("expected graphics backend argument");

        // declare where the libraries will be stored
        DynamicLibrary::Registry libs;

        // Attempt to load the requested path as a dynamic library first...
        DynamicLibrary &defaultGraphicsLib(DynamicLibrary::load(args[1], libs));
        IDisplay *defaultGraphics;

        // ... then try load it as a graphics lib.
        try {
            defaultGraphics = defaultGraphicsLib.symbol<IDisplay::EntryPoint>(IDisplay::ENTRY_POINT)();
        } catch (DynamicLibrary::UnknownSymbolError &) {
            std::ostringstream msg;

            msg << '\'' << args[1] << "' is not a valid graphics backend";
            throw std::runtime_error(msg.str());
        }

        // Load all the remaining libs in the default folder (the lib in args[1] may not be in this dir)
        DynamicLibrary::loadDirectory("./lib", libs);

        {
            Core core(libs, defaultGraphics);

            // Run the game(s)
            core.run();
        }

        return 0;
    }
} // namespace arcade
