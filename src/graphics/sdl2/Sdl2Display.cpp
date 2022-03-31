#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include <SDL.h>
#include <SDL_blendmode.h>
#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_rect.h>
#include <SDL_ttf.h>

#include <arcade/Color.hpp>
#include <arcade/Event.hpp>
#include <arcade/IAsset.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/graphics/units.hpp>
#include <arcade/types.hpp>

#include "Sdl2Display.hpp"
#include "asset/Font.hpp"
#include "asset/Texture.hpp"
#include "event.hpp"
#include "object/Rectangle.hpp"

namespace arcade
{
    Sdl2Display::Error::Error(std::string_view cause, char const *(*getError)())
        : std::runtime_error(std::string(cause) + ": " + (*getError)())
    {
    }

    Sdl2Display::Sdl2Display() : _window(nullptr), _renderer(nullptr), _size({0, 0})
    {
        // do nothing here, real init is done in ::setup()
    }

    void Sdl2Display::setup()
    {
        // SDL Libraries Initialization

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
            throw Error("failed to initialize SDL2");

        int initializedImgs(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG));
        if ((initializedImgs & IMG_INIT_JPG) == 0 || (initializedImgs & IMG_INIT_PNG) == 0)
            throw Error("failed to initialize SDL2 image", &IMG_GetError);

        if (TTF_Init())
            throw Error("failed to initialize SDL2 TTF", &TTF_GetError);

        // Window Creation

        this->_window = SDL_CreateWindow("Arcade (SDL2)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOW_SIZE_PIXELS.x, WINDOW_SIZE_PIXELS.y, SDL_WINDOW_RESIZABLE);
        if (!this->_window)
            throw Error("failed to initialize window");

        this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
        if (!this->_renderer)
            throw Error("failed to initialize window");
        SDL_SetRenderDrawBlendMode(this->_renderer, SDL_BLENDMODE_BLEND);
        this->updateSize(WINDOW_SIZE_UNITS);
        std::cout << "[sdl2]: setup" << std::endl;
    }

    void Sdl2Display::close()
    {
        std::cout << "[sdl2]: close" << std::endl;
        if (this->_renderer)
            SDL_DestroyRenderer(this->_renderer);
        if (this->_window)
            SDL_DestroyWindow(this->_window);

        if (TTF_WasInit())
            TTF_Quit();
        if (SDL_WasInit(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            IMG_Quit();
            SDL_Quit();
        }
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
            case IAsset::Type::Font: return Font::fromFile(path);
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

            std::cout << "Resized to " << this->_size.x << "x" << this->_size.y << " units" << std::endl;
        }

        return true;
    }

    void Sdl2Display::clear(Color color, DefaultColor)
    {
        SDL_RenderClear(this->_renderer);

        // only fill the screen if the requested color is not fully transparent
        if (color.a != Color::Transparent.a) {
            SDL_Rect screenRect{0, 0, static_cast<int>(this->_size.x), static_cast<int>(this->_size.y)};

            SDL_SetRenderDrawColor(this->_renderer, std::to_integer<uint8_t>(color.r),
                std::to_integer<uint8_t>(color.g), std::to_integer<uint8_t>(color.b),
                255 - std::to_integer<uint8_t>(color.a));
            SDL_RenderFillRect(this->_renderer, &screenRect);
            SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 0);
        }
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
        Font const *f = dynamic_cast<Font const *>(font);

        if (font != nullptr && f == nullptr)
            throw std::logic_error("textObject asset must be of font type");
        return Rectangle::create(this->_renderer, f, text);
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
            this->_size = toUnits(static_cast<vec2u>(vec2i{std::max(0, w), std::max(0, h)}));
    }
} // namespace arcade
