/// @file
/// @internal

#ifndef ARCADE_CORE_HPP_
#define ARCADE_CORE_HPP_

#include <string>
#include <unordered_map>

#include "util/DynamicLibrary.hpp"

namespace arcade
{
    class IDisplay;
    class IGame;

    class Core {
      public:
        Core(DynamicLibrary::Registry &libs, IDisplay *startingDisplay);

        Core(Core const &) = delete;
        Core(Core &&) = delete;

        /// Starts the program's main event loop.
        void eventLoop();

      private:
        std::unordered_map<std::string, IDisplay *> _displays;
        std::unordered_map<std::string, IGame *> _games;

        IDisplay *_currentDisplay;
        IGame *_currentGame;
    };
} // namespace arcade

#endif // !defined(ARCADE_CORE_HPP_)
