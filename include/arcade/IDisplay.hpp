/*
** EPITECH PROJECT, 2022
** Arcade: Interface
** File description:
** The display interface
*/

/// @file
///
/// The display interface.

#ifndef ARCADE_IDISPLAY_HPP_
#define ARCADE_IDISPLAY_HPP_

/// Entry point to get an instance of IDisplay
#define ARCADE_DISPLAY_ENTRY_POINT extern "C" ::arcade::IDisplay *arcade_DisplayEntryPoint()

#include <functional>
#include <memory>
#include <string_view>

#include "Color.hpp"
#include "IAsset.hpp"
#include "types.hpp"

namespace arcade
{
    class IGameObject;
    struct Event;
    class IRenderer;
    class IAssetManager;

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

        /// Type of the entry point of the library to get an instance of IGame.
        /// The function used as EntryPoint must be named as the DisplayEntryPointName below.
        using EntryPoint = IDisplay *(*)();
        /// Expected name of the Display entry point.
        static constexpr std::string_view ENTRY_POINT = "arcade_DisplayEntryPoint";

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
        virtual Type getType() const = 0;

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

        /// Clears the render target by filling with the given color.
        ///
        /// @note This function must be called before IDisplay::render().
        /// @note Calling this method without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        ///
        /// @param color 32-bit ARGB color to set.
        /// @param backup color to set if the display doesn't support 32-bit ARGB colors.
        ///
        virtual void clear(Color color, DefaultColor backup) = 0;

        /// Renders objects.
        ///
        /// @note Calling this method without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        ///
        /// Nothing is shown to the user until IDisplay::display() is called.
        ///
        /// @param drawer A function that renders game objects using the supplied renderer.
        virtual void render(std::function<void(IRenderer &)> drawer) = 0;

        /// Displays the rendered frame to the screen
        ///
        /// @note Calling this method without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        ///
        /// The rendered frame is immediately shown to the user at the end of the call.
        virtual void display() = 0;

        /// Calls a game's asset loading function.
        ///
        /// @note Calling this method without calling IDisplay::setup() leads to <b>undefined behavior</b>.
        ///
        /// @param loader A function that loads assets and game objects using the supplied asset manager.
        virtual void loadAssets(std::function<void(IAssetManager &)> loader) = 0;
    };
} // namespace arcade

#endif // !defined(ARCADE_DISPLAY_HPP_)
