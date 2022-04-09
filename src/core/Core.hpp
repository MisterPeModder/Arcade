/// @file
/// @internal

#ifndef ARCADE_CORE_HPP_
#define ARCADE_CORE_HPP_

#include <chrono>
#include <string>
#include <unordered_map>

#include <arcade/IDisplay.hpp>
#include <arcade/IGame.hpp>

#include "MainMenu.hpp"
#include "util/DynamicLibrary.hpp"
#include "util/LibraryInstance.hpp"
#include "util/LibrarySelector.hpp"

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
        // Main Loop
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Starts the program's main event loop.
        void run();

      private:
        /// Contains every display instance in the `libs` directory, may not contain the instance requested in the
        /// program's parameter.
        LibrarySelector<IDisplay> _displays;
        /// Contains every game instance in the `libs` directory.
        LibrarySelector<IGame> _games;

        /// IGame implementation used for rendering the main menu.
        /// This is not part of a dynamic library.
        MainMenu _mainMenu;

        /// The current display implementation.
        LibraryInstance<IDisplay> _display;
        /// The current game implementation.
        LibraryInstance<IGame> _game;

        std::chrono::steady_clock _clock;
        /// Timestamp of the last call to `Core::updateGame()`.
        std::chrono::time_point<std::chrono::steady_clock> _lastUpdate;
        /// Timestamp of the last call to `Core::render()`.
        std::chrono::time_point<std::chrono::steady_clock> _lastFrame;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // State Update
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Updates the active display based on the current selection.
        void updateDisplaySelection();

        /// Updates the active game based on the current selection.
        void updateGameSelection();

        /// (Re-)loads the assets and game objects of the active game or main menu.
        void reloadAssets();

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Main Loop
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Forwards events from the active display to the active game (or main menu if not there).
        ///
        /// @returns Whether the main event loop should proceed, if false, the loop is terminated.
        bool forwardEvents();

        /// Updates the active game instance, mostly though its `update()` method.
        ///
        /// @returns Whether the main event loop should proceed, if false, the loop skips to the next cycle.
        bool updateGame();

        /// Renders a frame of the active game to the screen.
        void render();

        /// Blocks the main thread until enough time has elapsed to limit the game to `FRAMERATE_LIMIT` fps.
        void awaitNextFrame();
    };
} // namespace arcade

#endif // !defined(ARCADE_CORE_HPP_)
