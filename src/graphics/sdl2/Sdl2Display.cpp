#include <algorithm>
#include <cstddef>
#include <cstdint>
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
#include <arcade/IDisplay.hpp>
#include <arcade/graphics/units.hpp>
#include <arcade/types.hpp>

#include "AssetManager.hpp"
#include "Renderer.hpp"
#include "Sdl2Display.hpp"
#include "event.hpp"

namespace arcade
{
    class IAssetManager;
    class IRenderer;

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

    void Sdl2Display::render(std::function<void(IRenderer &)> drawer)
    {
        Renderer renderer;
        drawer(renderer);
    }

    void Sdl2Display::display()
    {
        SDL_RenderPresent(this->_renderer);
    }

    void Sdl2Display::loadAssets(std::function<void(IAssetManager &)> loader)
    {
        AssetManager manager(this->_renderer);
        loader(manager);
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
