#include <iostream>
#include <stdexcept>
#include <string>

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>

#include <arcade/IAsset.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/types.hpp>

#include "Sdl2Display.hpp"
#include "event.hpp"

namespace arcade
{
    Sdl2Display::Error::Error(std::string_view cause) : std::runtime_error(std::string(cause) + ": " + SDL_GetError())
    {
    }

    struct Event;

    Sdl2Display::Sdl2Display() : _window(nullptr)
    {
        // do nothing here, real init is done in ::setup()
    }

    void Sdl2Display::setup()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
            throw Error("failed to initialize SDL2");

        this->_window = SDL_CreateWindow(
            "Arcade (SDL2)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_RESIZABLE);
        if (!this->_window)
            throw Error("failed to initialize window");
        std::cout << "loaded" << std::endl;
    }

    void Sdl2Display::close()
    {
        std::cout << "[sdl2]: "
                  << "closing" << std::endl;
        if (this->_window)
            SDL_DestroyWindow(this->_window);
        SDL_Quit();
    }

    IDisplay::Type Sdl2Display::getType() const
    {
        return IDisplay::Type::Graphical2D;
    }

    std::unique_ptr<IAsset> Sdl2Display::loadAsset(std::string_view name, IAsset::Type type)
    {
        (void)name;
        (void)type;
        return std::unique_ptr<IAsset>();
    }

    vec2u Sdl2Display::getSize() const
    {
        return {0, 0};
    }

    bool Sdl2Display::pollEvent(Event &event)
    {
        SDL_Event rawEvent;

        if (!SDL_PollEvent(&rawEvent))
            return false;

        return arcade::translateSdl2Event(rawEvent, event);
    }

    void Sdl2Display::render()
    {
    }

    void Sdl2Display::drawGameObject(const IGameObject &object)
    {
        (void)object;
    }

    std::unique_ptr<IGameObject> Sdl2Display::createTextObject(std::string_view text, IAsset const *font) const
    {
        (void)text;
        (void)font;
        return std::unique_ptr<IGameObject>();
    }

    std::unique_ptr<IGameObject> Sdl2Display::createRectObject(vec2u size, IAsset const *texture) const
    {
        (void)size;
        (void)texture;
        return std::unique_ptr<IGameObject>();
    }
} // namespace arcade
