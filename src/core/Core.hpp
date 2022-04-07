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
    /// Frames per second limit.
    constexpr unsigned int FRAMERATE_LIMIT = 60;

    class Core {
      public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Instantiation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        explicit Core(DynamicLibrary::Registry &libs, IDisplay *startingDisplay);

        Core(Core const &) = delete;
        Core(Core &&) = delete;

        ~Core() = default;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Runtime
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Starts the program's main event loop.
        void eventLoop();

      private:
        std::unordered_map<std::string, IDisplay *> _displays;
        std::unordered_map<std::string, IGame *> _games;

        /// The current display implementation.
        LibraryInstance<IDisplay> _display;
        /// The current game implementation.
        LibraryInstance<IGame> _game;

        /// Updates the current game's screen size by firing a synthetic @c Resized event.
        void updateGameSize();
    };
} // namespace arcade

#endif // !defined(ARCADE_CORE_HPP_)
