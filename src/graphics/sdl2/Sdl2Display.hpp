/// @file
/// @internal
///
/// SDL2 Graphics Backend.

#ifndef ARCADE_GRAPHICS_SDL2_SDL2_DISPLAY_HPP_
#define ARCADE_GRAPHICS_SDL2_SDL2_DISPLAY_HPP_

#include <functional>
#include <stdexcept>
#include <string_view>

#include <SDL_error.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include <arcade/Color.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    class IAssetManager;
    class IRenderer;
    struct Event;

    /// SDL2 Graphics implementation.
    class Sdl2Display : public IDisplay {
      public:
        class Error : public std::runtime_error {
          private:
            Error(std::string_view cause, char const *(*getError)() = &SDL_GetError);

            friend Sdl2Display;
        };

        Sdl2Display();

        void setup() override final;

        void close() override final;

        Type getType() const override final;

        vec2u getSize() const override final;

        bool pollEvent(Event &event) override final;

        void clear(Color color, DefaultColor backup) override final;

        void render(std::function<void(IRenderer &)> drawer) override final;

        void display() override final;

        void loadAssets(std::function<void(IAssetManager &)> loader) override final;

      private:
        SDL_Window *_window;
        SDL_Renderer *_renderer;
        vec2u _size;

        void updateSize(vec2u defaultSize);
    };
} // namespace arcade

#endif //! defined(ARCADE_GRAPHICS_SDL2_SDL2_DISPLAY_HPP_)
