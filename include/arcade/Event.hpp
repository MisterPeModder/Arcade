/*
** EPITECH PROJECT, 2022
** Arcade
** File description:
** Event
*/

#ifndef EVENT_HPP_
#define EVENT_HPP_

namespace arcade
{
    struct Event {
        using Key = char;

        enum class MouseButton {
            Left,   ///< The left mouse button
            Right,  ///< The right mouse button
            Middle, ///< The middle (wheel) mouse button

            ButtonCount ///< Keep last -- the total number of mouse buttons
        };

        ///  Size events parameters (Resized)
        struct SizeEvent {
            unsigned int width;  ///< New width, in pixels
            unsigned int height; ///< New height, in pixels
        };

        ///  Keyboard event parameters (KeyPressed, KeyReleased)
        struct KeyEvent {
            bool alt;     ///< Is the Alt key pressed?
            bool control; ///< Is the Control key pressed?
            bool shift;   ///< Is the Shift key pressed?
            bool system;  ///< Is the System key pressed?
            Key code;     ///< Code of the key that has been pressed
        };

        ///  Mouse move event parameters (MouseMoved)
        struct MouseMoveEvent {
            int x; ///< X position of the mouse pointer, relative to the left of the owner window
            int y; ///< Y position of the mouse pointer, relative to the top of the owner window
        };

        ///  Mouse buttons events parameters
        ///        (MouseButtonPressed, MouseButtonReleased)
        struct MouseButtonEvent {
            MouseButton button; ///< Code of the button that has been pressed
            int x;              ///< X position of the mouse pointer, relative to the left of the owner window
            int y;              ///< Y position of the mouse pointer, relative to the top of the owner window
        };

        ///  Enumeration of the different types of events
        enum class EventType {
            Closed,              ///< The window requested to be closed (no data)
            Resized,             ///< The window was resized (data in event.size)
            KeyPressed,          ///< A key was pressed (data in event.key)
            KeyReleased,         ///< A key was released (data in event.key)
            MouseButtonPressed,  ///< A mouse button was pressed (data in event.mouseButton)
            MouseButtonReleased, ///< A mouse button was released (data in event.mouseButton)
            MouseMoved,          ///< The mouse cursor moved (data in event.mouseMove)

            Count ///< Keep last -- the total number of event types
        };

        // Member data

        EventType type; ///< Type of the event

        union {
            SizeEvent size;           ///< Size event parameters (Event::Resized)
            KeyEvent key;             ///< Key event parameters (Event::KeyPressed, Event::KeyReleased)
            MouseMoveEvent mouseMove; ///< Mouse move event parameters (Event::MouseMoved)
            MouseButtonEvent mouseButton; ///< Mouse button event parameters (Event::MouseButtonPressed, Event::MouseButtonReleased)
        };
    };
} // namespace arcade

#endif /* !EVENT_HPP_ */
