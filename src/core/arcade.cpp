#include <iostream>
#include <span>
#include <string>

#include <arcade/Event.hpp>
#include <arcade/IDisplay.hpp>

#include "core.hpp"
#include "util/DynamicLibrary.hpp"

namespace arcade
{
    static void mainLoop(IDisplay &display)
    {
        Event event;

        for (;;) {
            while (display.pollEvent(event)) {
                if (event.type == Event::EventType::Closed)
                    return;
            }

            display.render();
        }
    }

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

        mainLoop(*defaultGraphics);

        return 0;
    }
} // namespace arcade
