/*
** EPITECH PROJECT, 2022
** Arcade: Interface
** File description:
** Event data structure
*/

/// @file
///
/// The Event data structure.

#ifndef ARCADE_EVENT_HPP_
#define ARCADE_EVENT_HPP_

namespace arcade
{
    /// Contains an event's data.
    ///
    /// Event data is fetched by first checking its type and then accessing the correspoding member.
    struct Event {
        /// Key code for keyboard events.
        ///
        /// Keys are represented by their corresponding character, meaning that some keys are not currently
        /// representable using this format (such as the Enter key).
        using Key = char;

        /// Represents a mouse button.
        enum class MouseButton {
            /// The left mouse button.
            Left,
            /// The right mouse button.
            Right,
            /// The middle (wheel) mouse button.
            Middle,

            /// Keep last -- the total number of mouse buttons.
            Count
        };

        /// Size events parameters (EventType::Resized).
        struct SizeEvent {
            /// New width, in units.
            unsigned int width;
            /// New height, in units.
            unsigned int height;
        };

        /// @brief Keyboard event parameters (EventType::KeyPressed, EventType::KeyReleased).
        struct KeyEvent {
            /// Is the Alt key pressed?
            bool alt;
            /// Is the Control key pressed?
            bool control;
            /// Is the Shift key pressed?
            bool shift;
            /// Is the System key pressed?
            bool system;
            /// Code of the key that has been pressed.
            Key code;
        };

        /// Mouse move event parameters (EventType::MouseMoved)
        struct MouseMoveEvent {
            /// X position of the mouse pointer, relative to the left of the owner window.
            int x;
            /// Y position of the mouse pointer, relative to the top of the owner window.
            int y;
        };

        /// Mouse buttons events parameters (EventType::MouseButtonPressed, EventType::MouseButtonReleased).
        struct MouseButtonEvent {
            /// Code of the button that has been pressed.
            MouseButton button;
            /// X position of the mouse pointer, relative to the left of the owner window.
            int x;
            /// Y position of the mouse pointer, relative to the top of the owner window.
            int y;
        };

        /// Enumeration of the different types of events.
        enum class EventType {
            /// The window requested to be closed (no data).
            Closed,
            /// The window was resized (data in Event.size).
            Resized,
            /// A key was pressed (data in Event.key).
            KeyPressed,
            /// A key was released (data in Event.key).
            KeyReleased,
            /// A mouse button was pressed (data in Event.mouseButton).
            MouseButtonPressed,
            /// A mouse button was released (data in Event.mouseButton).
            MouseButtonReleased,
            /// The mouse cursor moved (data in Event.mouseMove).
            MouseMoved,

            /// Keep last -- the total number of event types.
            Count
        };

        // Member data

        /// Type of the event.
        EventType type;

        union {
            /// Size event parameters (Event::Resized).
            SizeEvent size;
            /// Key event parameters (Event::KeyPressed, Event::KeyReleased).
            KeyEvent key;
            /// Mouse move event parameters (Event::MouseMoved).
            MouseMoveEvent mouseMove;
            /// Mouse button event parameters (Event::MouseButtonPressed, Event::MouseButtonReleased).
            MouseButtonEvent mouseButton;
        };
    };
} // namespace arcade

#endif // !defined(ARCADE_EVENT_HPP_)
