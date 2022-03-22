/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SDL2_OBJECT_RECTANGLE_HPP_
#define ARCADE_GRAPHICS_SDL2_OBJECT_RECTANGLE_HPP_

#include <memory>

#include <SDL_rect.h>
#include <SDL_render.h>

#include <arcade/Color.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    class Font;
    class Texture;

    class Rectangle : public IGameObject {
      public:
        Rectangle();

        Rectangle(Rectangle const &) = delete;

        Rectangle(Rectangle &&);
        Rectangle &operator=(Rectangle &&);

        ~Rectangle();

        void draw() const;

        static std::unique_ptr<Rectangle> create(SDL_Renderer *renderer, Texture const *texture, vec2u size);
        static std::unique_ptr<Rectangle> create(SDL_Renderer *renderer, Font const *font, std::string_view text);

        Type getType() const override final;
        vec2u getSize() const override final;
        vec2i getPosition() const override final;
        void setPosition(vec2i pos) override final;
        void setForeground(Color color, DefaultColor) override final;
        void setBackground(Color color, DefaultColor) override final;

      private:
        /// Position & Size (in pixels, not units)
        SDL_Rect _dims;
        SDL_Renderer *_renderer;
        SDL_Texture *_sprite;

        Color _foregroundColor;
        Color _backgroundColor;

        Rectangle(SDL_Renderer *renderer, SDL_Texture *sprite, vec2u size);

        void drawColor(Color color) const;
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_OBJECT_RECTANGLE_HPP_)
