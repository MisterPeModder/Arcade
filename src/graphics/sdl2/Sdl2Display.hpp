/// @file
/// @internal
///
/// SDL2 Graphics Backend.

#ifndef ARCADE_GRAPHICS_SDL2_SDL2_DISPLAY_HPP_
#define ARCADE_GRAPHICS_SDL2_SDL2_DISPLAY_HPP_

#include <memory>
#include <stdexcept>
#include <string_view>

#include <SDL_video.h>

#include <arcade/IAsset.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    class IGameObject;
    struct Event;

    /// SDL2 Graphics implementation.
    class Sdl2Display : public IDisplay {
      public:
        class Error : public std::runtime_error {
          private:
            Error(std::string_view cause);

            friend Sdl2Display;
        };

        Sdl2Display();

        void setup() override final;

        void close() override final;

        Type getType() const override final;

        std::unique_ptr<IAsset> loadAsset(std::string_view name, IAsset::Type type) override final;

        vec2u getSize() const override final;

        virtual bool pollEvent(Event &event) override final;

        void render() override final;

        void drawGameObject(const IGameObject &object) override final;

        std::unique_ptr<IGameObject> createTextObject(std::string_view text, IAsset const *font) const override final;

        std::unique_ptr<IGameObject> createRectObject(vec2u size, IAsset const *texture) const override final;

      private:
        SDL_Window *_window;
    };
} // namespace arcade

#endif //! defined(ARCADE_GRAPHICS_SDL2_SDL2_DISPLAY_HPP_)
