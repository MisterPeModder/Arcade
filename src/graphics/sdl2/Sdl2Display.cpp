#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>

#include <arcade/Event.hpp>
#include <arcade/IAsset.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/types.hpp>

#include "Sdl2Display.hpp"
#include "asset/Texture.hpp"
#include "event.hpp"
#include "object/Rectangle.hpp"

namespace arcade
{
    Sdl2Display::Error::Error(std::string_view cause) : std::runtime_error(std::string(cause) + ": " + SDL_GetError())
    {
    }

    Sdl2Display::Sdl2Display() : _window(nullptr), _renderer(nullptr), _size({0, 0})
    {
        // do nothing here, real init is done in ::setup()
    }

    void Sdl2Display::setup()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
            throw Error("failed to initialize SDL2");

        this->_window = SDL_CreateWindow("Arcade (SDL2)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            DEFAULT_SIZE.x, DEFAULT_SIZE.y, SDL_WINDOW_RESIZABLE);
        if (!this->_window)
            throw Error("failed to initialize window");

        this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
        if (!this->_renderer)
            throw Error("failed to initialize window");
        SDL_SetRenderDrawBlendMode(this->_renderer, SDL_BLENDMODE_BLEND);
        this->updateSize(DEFAULT_SIZE);
        std::cout << "[sdl2]: setup" << std::endl;
    }

    void Sdl2Display::close()
    {
        std::cout << "[sdl2]: close" << std::endl;
        if (this->_renderer)
            SDL_DestroyRenderer(this->_renderer);
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
        std::filesystem::path path(name);

        switch (type) {
            case IAsset::Type::Texture: return Texture::fromFile(path);
            default: return std::unique_ptr<IAsset>();
        }
    }

    vec2u Sdl2Display::getSize() const
    {
        return this->_size;
    }

    bool Sdl2Display::pollEvent(Event &event)
    {
        SDL_Event rawEvent;

        if (!SDL_PollEvent(&rawEvent) || !arcade::translateSdl2Event(rawEvent, event))
            return false;

        // If the window was resized, query the size of the render output and update the event before returning.
        if (event.type == Event::Type::Resized) {
            event.size.oldSize = this->_size;
            this->updateSize(event.size.newSize);
            event.size.newSize = this->_size;

            std::cout << "Resized to " << this->_size.x << "x" << this->_size.y << "px" << std::endl;
        }

        return true;
    }

    void Sdl2Display::render()
    {
        SDL_RenderPresent(this->_renderer);
        SDL_RenderClear(this->_renderer);
    }

    void Sdl2Display::drawGameObject(IGameObject const &object)
    {
        Rectangle const *rect = dynamic_cast<Rectangle const *>(&object);

        if (rect)
            rect->draw();
    }

    std::unique_ptr<IGameObject> Sdl2Display::createTextObject(std::string_view text, IAsset const *font) const
    {
        (void)text;
        (void)font;
        return std::unique_ptr<IGameObject>();
    }

    std::unique_ptr<IGameObject> Sdl2Display::createRectObject(vec2u size, IAsset const *texture) const
    {
        Texture const *t = dynamic_cast<Texture const *>(texture);

        if (texture != nullptr && t == nullptr)
            throw std::logic_error("rectObject asset must be of texture type");
        return Rectangle::create(this->_renderer, t, size);
    }

    void Sdl2Display::updateSize(vec2u defaultSize)
    {
        int w, h;

        if (SDL_GetRendererOutputSize(this->_renderer, &w, &h) != 0)
            this->_size = defaultSize;
        else
            this->_size = {static_cast<unsigned int>(std::max(0, w)), static_cast<unsigned int>(std::max(0, h))};
    }
} // namespace arcade
