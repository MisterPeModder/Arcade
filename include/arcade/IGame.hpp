/*
** EPITECH PROJECT, 2022
** Arcade: Interface
** File description:
** The game interface
*/

/// @file
///
/// The game interface.

#ifndef ARCADE_IGAME_HPP_
#define ARCADE_IGAME_HPP_

/// Entry point to get an instance of IGame
#define ARCADE_GAME_ENTRY_POINT extern "C" ::arcade::IGame *arcade_GameEntryPoint()

#include <string_view>

namespace arcade
{
    class IDisplay;
    struct Event;

    /// A game instance.
    class IGame {
      public:
        /// A game's state.
        enum class State {
            /// The initial state of the game.
            Loaded,
            /// After the first update.
            Running,
            /// When a previously running game is paused.
            Paused,
            /// When the game is ended.
            Ended,
        };

        /// Type of the entry point of the library to get an instance of IGame.
        /// The function used as EntryPoint must be named as the GameEntryPointName below.
        using EntryPoint = IGame *(*)();
        /// Expected name of the Game entry point.
        static constexpr std::string_view ENTRY_POINT = "arcade_GameEntryPoint";

        virtual ~IGame() = default;

        /// Intializes this game.
        ///
        /// Each call to IGame::setup() @b must be followed by a call to IGame::close().
        /// Calling this function again without calling IGame::close() leads to <b>undefined behavior</b>.
        ///
        /// @note Calling this method without calling IGame::setup() leads to <b>undefined behavior</b>.
        ///
        /// @param display The starting display manager.
        virtual void setup(IDisplay &display) = 0;

        /// Releases the ressources allocated by this game.
        ///
        /// Each call to IGame::close() @b must be preceded by a call to IGame::setup().
        /// Calling this function again without calling IGame::setup() leads to <b>undefined behavior</b>.
        ///
        /// @note Calling this method without calling IGame::setup() leads to <b>undefined behavior</b>.
        virtual void close() = 0;

        /// Changes the display of the game.
        ///
        /// Changes in display must not interrupt the game.
        ///
        /// @note Calling this method without calling IGame::setup() leads to <b>undefined behavior</b>.
        ///
        /// @param display The new display manager.
        virtual void setDisplay(IDisplay &display) = 0;

        /// Alters the state of the game.
        ///
        /// @note Calling this method without calling IGame::setup() leads to <b>undefined behavior</b>.
        ///
        /// @param state The new state.
        virtual void setState(State state) = 0;

        /// @note Calling this method without calling IGame::setup() leads to <b>undefined behavior</b>.
        ///
        /// @returns The current state.
        virtual State getState() const = 0;

        /// @note Calling this method without calling IGame::setup() leads to <b>undefined behavior</b>.
        ///
        /// @returns The current score of the player.
        virtual unsigned int getScore() const = 0;

        /// Updates the game logic.
        ///
        /// @note Calling this method without calling IGame::setup() leads to <b>undefined behavior</b>.
        ///
        /// @param delta The time (in seconds) elapsed since the last update.
        virtual void update(float delta) = 0;

        /// Handles the given event.
        ///
        /// @note Calling this method without calling IGame::setup() leads to <b>undefined behavior</b>.
        ///
        /// @param event The event to process.
        virtual void handleEvent(Event &event) = 0;
    };
} // namespace arcade

#endif // !defined(ARCADE_IGAME_HPP_)
