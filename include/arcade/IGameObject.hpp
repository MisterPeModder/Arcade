/*
** EPITECH PROJECT, 2022
** Arcade: Interface
** File description:
** Game object interface
*/

/// @file
///
/// Game object interface.

#ifndef ARCADE_GAME_OBJECT_HPP_
#define ARCADE_GAME_OBJECT_HPP_

#include "Color.hpp"
#include "types.hpp"

namespace arcade
{
    /// A movable, drawable game object such as text or sprites
    class IGameObject {
      public:
        /// The type of a game object.
        enum class Type {
            /// Textual object.
            Text,
            /// Textured rectangle object.
            Rect,
        };

        virtual ~IGameObject() = default;

        /// @returns The type of this object.
        virtual Type getType() const = 0;

        /// @returns The dimensions (in units) of this object.
        virtual vec2u getSize() const = 0;

        /// TODO: we might want to change to type to an vec2f instead.
        /// @returns The position (in units) of thiss object.
        virtual vec2i getPosition() const = 0;

        /// Moves the game object.
        ///
        /// @param pos The new position.
        virtual void setPosition(vec2i pos) = 0;

        /// Change the foreground color of the game object.
        ///
        /// @param color 32-bit ARGB Color to set.
        /// @param backup 8bit color to set if the display doesn't support 32-bit ARGB colors.
        ///
        virtual void setForeground(Color color, DefaultColor backup = DefaultColor::White) = 0;

        /// Change the background color of the game object.
        ///
        /// @param color 32-bit ARGB color to set.
        /// @param backup 8bit color to set if the display doesn't support 32-bit ARGB colors.
        ///
        virtual void setBackground(Color color, DefaultColor backup = DefaultColor::Transparent) = 0;
    };
} // namespace arcade

#endif // !defined(ARCADE_GAME_OBJECT_HPP_)
