#include <algorithm>

#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_mouse.h>
#include <SDL_video.h>

#include <arcade/Event.hpp>
#include <arcade/types.hpp>

#include "event.hpp"

namespace arcade
{
    static char convertKeyCode(SDL_Keycode keyCode)
    {
        switch (keyCode) {
            case SDLK_BACKSPACE:
            case SDLK_KP_BACKSPACE: return '\b';
            case SDLK_TAB: return '\t';
            case SDLK_RETURN:
            case SDLK_RETURN2:
            case SDLK_KP_ENTER: return '\n';
            case SDLK_ESCAPE: return '\x1b';
            case SDLK_SPACE:
            case SDLK_KP_SPACE: return ' ';
            case SDLK_EXCLAIM:
            case SDLK_KP_EXCLAM: return '!';
            case SDLK_QUOTEDBL: return '"';
            case SDLK_HASH:
            case SDLK_KP_HASH: return '#';
            case SDLK_DOLLAR: return '$';
            case SDLK_PERCENT:
            case SDLK_KP_PERCENT: return '%';
            case SDLK_AMPERSAND: return '&';
            case SDLK_QUOTE: return '\'';
            case SDLK_LEFTPAREN:
            case SDLK_KP_LEFTPAREN: return '(';
            case SDLK_RIGHTPAREN:
            case SDLK_KP_RIGHTPAREN: return ')';
            case SDLK_ASTERISK:
            case SDLK_KP_MULTIPLY: return '*';
            case SDLK_PLUS:
            case SDLK_KP_PLUS: return '+';
            case SDLK_COMMA:
            case SDLK_KP_COMMA: return ',';
            case SDLK_MINUS:
            case SDLK_KP_MINUS: return '-';
            case SDLK_PERIOD:
            case SDLK_KP_PERIOD: return '.';
            case SDLK_SLASH:
            case SDLK_KP_DIVIDE: return '/';
            case SDLK_0:
            case SDLK_KP_0: return '0';
            case SDLK_KP_00: return '0';
            case SDLK_KP_000: return '0';
            case SDLK_1:
            case SDLK_KP_1: return '1';
            case SDLK_2:
            case SDLK_KP_2: return '2';
            case SDLK_3:
            case SDLK_KP_3: return '3';
            case SDLK_4:
            case SDLK_KP_4: return '4';
            case SDLK_5:
            case SDLK_KP_5: return '5';
            case SDLK_6:
            case SDLK_KP_6: return '6';
            case SDLK_7: return '7';
            case SDLK_KP_7: return '7';
            case SDLK_8:
            case SDLK_KP_8: return '8';
            case SDLK_9:
            case SDLK_KP_9: return '9';
            case SDLK_COLON:
            case SDLK_KP_COLON: return ':';
            case SDLK_SEMICOLON: return ';';
            case SDLK_LESS:
            case SDLK_KP_LESS: return '<';
            case SDLK_EQUALS: return '=';
            case SDLK_KP_EQUALS: return '=';
            case SDLK_GREATER:
            case SDLK_KP_GREATER: return '>';
            case SDLK_QUESTION: return '?';
            case SDLK_AT:
            case SDLK_KP_AT: return '@';
            case SDLK_a: return 'a';
            case SDLK_b: return 'b';
            case SDLK_c: return 'c';
            case SDLK_d: return 'd';
            case SDLK_e: return 'e';
            case SDLK_f: return 'f';
            case SDLK_g: return 'g';
            case SDLK_h: return 'h';
            case SDLK_i: return 'i';
            case SDLK_j: return 'j';
            case SDLK_k: return 'k';
            case SDLK_l: return 'l';
            case SDLK_m: return 'm';
            case SDLK_n: return 'n';
            case SDLK_o: return 'o';
            case SDLK_p: return 'p';
            case SDLK_q: return 'q';
            case SDLK_r: return 'r';
            case SDLK_s: return 's';
            case SDLK_t: return 't';
            case SDLK_u: return 'u';
            case SDLK_v: return 'v';
            case SDLK_w: return 'w';
            case SDLK_x: return 'x';
            case SDLK_y: return 'y';
            case SDLK_z: return 'z';
            case SDLK_LEFTBRACKET: return '[';
            case SDLK_BACKSLASH: return '\\';
            case SDLK_RIGHTBRACKET: return ']';
            case SDLK_CARET:
            case SDLK_KP_POWER: return '^';
            case SDLK_UNDERSCORE: return '_';
            case SDLK_BACKQUOTE: return '`';
            case SDLK_KP_LEFTBRACE: return '{';
            case SDLK_KP_VERTICALBAR: return '|';
            case SDLK_KP_RIGHTBRACE: return '}';
            default: return 0;
        }
    }

    static bool translateSdl2WindowEvent(SDL_WindowEvent const &raw, Event &event)
    {
        switch (raw.event) {
            case SDL_WINDOWEVENT_CLOSE: event.type = Event::Type::Closed; return true;
            case SDL_WINDOWEVENT_RESIZED:
                event.type = Event::Type::Resized;
                event.size.newSize.x = static_cast<unsigned int>(std::max(0, raw.data1));
                event.size.newSize.y = static_cast<unsigned int>(std::max(0, raw.data2));
                return true;
            default: return false;
        }
    }

    static bool translateSdl2MouseButtonEvent(SDL_MouseButtonEvent const &raw, Event &event)
    {
        if (raw.type == SDL_MOUSEBUTTONDOWN)
            event.type = Event::Type::MouseButtonPressed;
        else
            event.type = Event::Type::MouseButtonReleased;

        switch (raw.button) {
            case SDL_BUTTON_RIGHT:
            case SDL_BUTTON_X2: event.mouseButton.button = Event::MouseButton::Right; break;

            case SDL_BUTTON_MIDDLE: event.mouseButton.button = Event::MouseButton::Middle; break;

            case SDL_BUTTON_LEFT:
            case SDL_BUTTON_X1:
            default: event.mouseButton.button = Event::MouseButton::Left; break;
        }

        event.mouseButton.pos = {raw.x, raw.y};
        return true;
    }

    static bool translateSdl2MouseMotionEvent(SDL_MouseMotionEvent const &raw, Event &event)
    {
        event.type = Event::Type::MouseMoved;
        event.mouseMove.pos = {raw.x, raw.y};
        return true;
    }

    static bool translateSdl2KeyboardEvent(SDL_KeyboardEvent const &raw, Event &event)
    {
        event.key.code = convertKeyCode(raw.keysym.sym);
        if (event.key.code == 0)
            return false;

        if (raw.type == SDL_KEYDOWN)
            event.type = Event::Type::KeyPressed;
        else
            event.type = Event::Type::KeyReleased;

        auto mod = raw.keysym.mod;
        event.key.alt = !!(mod & KMOD_ALT);
        event.key.control = !!(mod & KMOD_CTRL);
        event.key.shift = !!(mod & KMOD_SHIFT);
        event.key.system = !!(mod & KMOD_GUI);

        return true;
    }

    bool translateSdl2Event(SDL_Event const &raw, Event &event)
    {
        switch (raw.type) {
            case SDL_WINDOWEVENT: return translateSdl2WindowEvent(raw.window, event);
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: return translateSdl2MouseButtonEvent(raw.button, event);
            case SDL_MOUSEMOTION: return translateSdl2MouseMotionEvent(raw.motion, event);
            case SDL_KEYUP:
            case SDL_KEYDOWN: return translateSdl2KeyboardEvent(raw.key, event);
            default: return false;
        }
    }
} // namespace arcade
