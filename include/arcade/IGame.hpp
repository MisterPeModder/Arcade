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

namespace arcade
{
    class Event;

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

        virtual ~IGame() = default;

        /// Intializes this game.
        ///
        /// Each call to IGame::setup() @b must be followed by a call to IGame::close().
        /// Calling this function again without calling IGame::close() leads to <b>undefined behavior</b>.
        ///
        /// @note Calling this method without calling IGame::setup() leads to <b>undefined behavior</b>.
        virtual void setup() = 0;

        /// Releases the ressources allocated by this game.
        ///
        /// Each call to IGame::close() @b must be preceded by a call to IGame::setup().
        /// Calling this function again without calling IGame::setup() leads to <b>undefined behavior</b>.
        ///
        /// @note Calling this method without calling IGame::setup() leads to <b>undefined behavior</b>.
        virtual void close() = 0;

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
