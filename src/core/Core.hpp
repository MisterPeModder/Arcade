/// @file
/// @internal

#ifndef ARCADE_CORE_HPP_
#define ARCADE_CORE_HPP_

#include <string>
#include <unordered_map>

#include <arcade/IDisplay.hpp>
#include <arcade/IGame.hpp>

#include "util/DynamicLibrary.hpp"
#include "util/LibraryInstance.hpp"

namespace arcade
{
    class Core {
      public:
        Core(DynamicLibrary::Registry &libs, IDisplay *startingDisplay);

        Core(Core const &) = delete;
        Core(Core &&) = delete;

        ~Core() = default;

        /// Starts the program's main event loop.
        void eventLoop();

      private:
        std::unordered_map<std::string, IDisplay *> _displays;
        std::unordered_map<std::string, IGame *> _games;

        LibraryInstance<IDisplay> _display;
        LibraryInstance<IGame> _game;
    };
} // namespace arcade

#endif // !defined(ARCADE_CORE_HPP_)
