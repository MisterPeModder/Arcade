#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <arcade/Event.hpp>
#include <arcade/graphics/units.hpp>
#include <arcade/types.hpp>

#include "event.hpp"

using sfEventType = sf::Event::EventType;
using KeyCode = sf::Keyboard::Key;

namespace arcade
{
    static char convertKeyCode(KeyCode keyCode)
    {
        switch (keyCode) {
            case KeyCode::Backspace: return '\b';
            case KeyCode::Tab: return '\t';
            case KeyCode::Enter: return '\n';
            case KeyCode::Escape: return '\x1b';
            case KeyCode::Space: return ' ';
            case KeyCode::Quote: return '\'';
            case KeyCode::Multiply: return '*';
            case KeyCode::Add: return '+';
            case KeyCode::Comma: return ',';
            case KeyCode::Subtract:
            case KeyCode::Hyphen: return '-';
            case KeyCode::Period: return '.';
            case KeyCode::Slash:
            case KeyCode::Divide: return '/';
            case KeyCode::Num0:
            case KeyCode::Numpad0: return '0';
            case KeyCode::Num1:
            case KeyCode::Numpad1: return '1';
            case KeyCode::Num2:
            case KeyCode::Numpad2: return '2';
            case KeyCode::Num3:
            case KeyCode::Numpad3: return '3';
            case KeyCode::Num4:
            case KeyCode::Numpad4: return '4';
            case KeyCode::Num5:
            case KeyCode::Numpad5: return '5';
            case KeyCode::Num6:
            case KeyCode::Numpad6: return '6';
            case KeyCode::Num7:
            case KeyCode::Numpad7: return '7';
            case KeyCode::Num8:
            case KeyCode::Numpad8: return '8';
            case KeyCode::Num9:
            case KeyCode::Numpad9: return '9';
            case KeyCode::SemiColon: return ':';
            case KeyCode::Equal: return '=';
            case KeyCode::A: return 'a';
            case KeyCode::B: return 'b';
            case KeyCode::C: return 'c';
            case KeyCode::D: return 'd';
            case KeyCode::E: return 'e';
            case KeyCode::F: return 'f';
            case KeyCode::G: return 'g';
            case KeyCode::H: return 'h';
            case KeyCode::I: return 'i';
            case KeyCode::J: return 'j';
            case KeyCode::K: return 'k';
            case KeyCode::L: return 'l';
            case KeyCode::M: return 'm';
            case KeyCode::N: return 'n';
            case KeyCode::O: return 'o';
            case KeyCode::P: return 'p';
            case KeyCode::Q: return 'q';
            case KeyCode::R: return 'r';
            case KeyCode::S: return 's';
            case KeyCode::T: return 't';
            case KeyCode::U: return 'u';
            case KeyCode::V: return 'v';
            case KeyCode::W: return 'w';
            case KeyCode::X: return 'x';
            case KeyCode::Y: return 'y';
            case KeyCode::Z: return 'z';
            case KeyCode::LBracket: return '[';
            case KeyCode::Backslash: return '\\';
            case KeyCode::RBracket: return ']';
            default: return false;
        }
    }

    static bool translateSfmlClosedEvent(sf::Event const &raw, Event &event)
    {
        (void)raw;
        event.type = Event::Type::Closed;
        return true;
    }

    static bool translateSfmlResizeEvent(sf::Event const &raw, Event &event)
    {
        event.type = Event::Type::Resized;
        event.size.newSize = toUnits(vec2u{raw.size.width, raw.size.height});
        return true;
    }

    static bool translateSfmlMouseButtonEvent(sf::Event const &raw, Event &event)
    {
        if (raw.type == sfEventType::MouseButtonPressed)
            event.type = Event::Type::MouseButtonPressed;
        else
            event.type = Event::Type::MouseButtonReleased;

        switch (raw.mouseButton.button) {
            case sf::Mouse::Button::Right:
            case sf::Mouse::Button::XButton2: event.mouseButton.button = Event::MouseButton::Right; break;

            case sf::Mouse::Button::Middle: event.mouseButton.button = Event::MouseButton::Middle; break;

            case sf::Mouse::Button::Left:
            case sf::Mouse::Button::XButton1:
            default: event.mouseButton.button = Event::MouseButton::Left; break;
        }

        event.mouseButton.pos = toUnits(vec2i{raw.mouseButton.x, raw.mouseButton.y});
        return true;
    }

    static bool translateSfmlMouseMotionEvent(sf::Event const &raw, Event &event)
    {
        event.type = Event::Type::MouseMoved;
        event.mouseMove.pos = toUnits(vec2i{raw.mouseMove.x, raw.mouseMove.y});
        return true;
    }

    static bool translateSfmlKeyboardEvent(sf::Event const &raw, Event &event)
    {
        event.key.code = convertKeyCode(raw.key.code);
        if (event.key.code == 0)
            return false;

        if (raw.type == sfEventType::KeyPressed)
            event.type = Event::Type::KeyPressed;
        else
            event.type = Event::Type::KeyReleased;

        event.key.alt = raw.key.alt;
        event.key.control = raw.key.control;
        event.key.shift = raw.key.shift;
        event.key.system = raw.key.system;

        return true;
    }

    bool translateSfmlEvent(sf::Event const &raw, Event &event)
    {
        switch (raw.type) {
            case sfEventType::Closed: return translateSfmlClosedEvent(raw, event);
            case sfEventType::Resized: return translateSfmlResizeEvent(raw, event);
            case sfEventType::MouseButtonPressed:
            case sfEventType::MouseButtonReleased: return translateSfmlMouseButtonEvent(raw, event);
            case sfEventType::MouseMoved: return translateSfmlMouseMotionEvent(raw, event);
            case sfEventType::KeyPressed:
            case sfEventType::KeyReleased: return translateSfmlKeyboardEvent(raw, event);
            default: return false;
        }
    }
} // namespace arcade
