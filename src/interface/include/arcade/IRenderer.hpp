/*
** EPITECH PROJECT, 2022
** Arcade: Interface
** File description:
** Game rendering
*/

/// @file
///
/// Game rendering.

#ifndef ARCADE_IRENDERER_HPP_
#define ARCADE_IRENDERER_HPP_

namespace arcade
{
    class IGameObject;

    /// Rendering interface.
    class IRenderer {
      public:
        virtual ~IRenderer() = default;

        /// Draws a game object to the display's internal buffer(s).
        ///
        /// Drawn IGameObject instances will not display until the next call to IDisplay::render().
        ///
        /// @param object The object to draw.
        virtual void draw(IGameObject const &object) = 0;
    };
} // namespace arcade

#endif // !defined(ARCADE_IRENDERER_HPP_)
