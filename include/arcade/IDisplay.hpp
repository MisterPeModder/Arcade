/*
** EPITECH PROJECT, 2022
** Arcade: Interface
** File description:
** The display interface
*/

/// @file
///
/// The display interface.

#ifndef IDISPLAY_HPP_
#define IDISPLAY_HPP_

#include <memory>
#include <string_view>

#include "IAsset.hpp"
#include "types.hpp"

namespace arcade
{
    class IGameObject;
    struct Event;

    /// Graphics backend.
    ///
    /// Instances of IDisplay are reponsible for managing the display, events and assets of IGame instances.
    class IDisplay {
      public:
        /// The graphics mode.
        enum class Type {
            /// Text-only output.
            Terminal,
            /// Flat graphical output.
            Graphical2D,
        };

        virtual ~IDisplay() = default;

        /// Intializes this graphics backend.
        ///
        /// Each call to IDisplay::setup() @b must be followed by a call to IDisplay::close().
        /// Calling this function again without calling IDisplay::close() leads to <b>undefined behavior</b>.
        virtual void setup() = 0;

        /// Releases the ressources allocated by this graphics backend.
        ///
        /// Each call to IDisplay::close() @b must be preceded by a call to IDisplay::setup().
        /// Calling this function again without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        virtual void close() = 0;

        /// @note Calling this method without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        ///
        /// @returns The type of display output.
        virtual Type getDisplayType() const = 0;

        /// Fetches an asset by name, loading it if necessary.
        ///
        /// The returned IAsset instance is a reference to the real underlying asset, when switching displays, all
        /// existing instances of IAsset will attempt to convert to equivalent assets for the new display mode.
        ///
        /// @note Calling this method without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        ///
        /// @param name The name of the requested asset.
        /// @param type Which type of asset to fetch?
        ///
        /// @returns A reference to the loaded asset, or a null reference if the requested asset failed to load.
        virtual std::unique_ptr<IAsset> loadAsset(std::string_view name, IAsset::Type type) = 0;

        /// @note Calling this method without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        ///
        /// @returns The size of the display, in units.
        virtual vec2u getSize() const = 0;

        /// Fetches the next event in the event queue, this operation is non-blocking.
        ///
        /// @note Calling this method without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        ///
        /// @param[out] event Where the event will be stored, may be uninitialized.
        ///
        /// @returns Whether an event was loaded into @c event, false means that the event queue is currently empty.
        virtual bool pollEvent(Event &event) = 0;

        /// Renders a frame and displays it.
        ///
        /// @note Calling this method without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        ///
        /// The rendered frame is immediately shown to the user at the end of the call.
        virtual void render() = 0;

        /// Draws a game object to the display's internal buffer(s).
        ///
        /// Drawn IGameObject instances will not display until the next call to IDisplay::render().
        ///
        /// @note Calling this method without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        ///
        /// @param object The object to draw.
        virtual void drawGameObject(const IGameObject &object) = 0;

        /// Creates a text (as in string) object instance.
        ///
        /// @note Calling this method without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        ///
        /// @param text The string to display.
        /// @param font The font to use.
        ///
        /// @throws std::logic_error When @c font is not a font asset.
        ///
        /// @returns A boxed IGameObject instance.
        virtual std::unique_ptr<IGameObject> createTextObject(std::string_view text, IAsset const &font) const = 0;

        /// Creates a textured rectangle object.
        ///
        /// @note Calling this method without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        ///
        /// @param size The dimensions (in units) of the rectangle.
        /// @param texture The texture to use.
        ///
        /// @throws std::logic_error When @c texture is not a texture asset.
        ///
        /// @returns A boxed IGameObject instance.
        virtual std::unique_ptr<IGameObject> createRectObject(vec2u size, IAsset const &texture) const = 0;
    };
} // namespace arcade

#endif // !defined(ARCADE_DISPLAY_HPP_)
