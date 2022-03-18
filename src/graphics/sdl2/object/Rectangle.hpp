/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SDL2_OBJECT_RECTANGLE_HPP_
#define ARCADE_GRAPHICS_SDL2_OBJECT_RECTANGLE_HPP_

#include <SDL_rect.h>

#include <arcade/Color.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    class Texture;

    class Rectangle : public IGameObject {
      public:
        Rectangle();
        Rectangle(vec2u size, Texture const *texture);

        Rectangle(Rectangle const &) = delete;

        Rectangle(Rectangle &&);
        Rectangle &operator=(Rectangle &&);

        void draw() const;

        Type getType() const override final;
        vec2u getSize() const override final;
        vec2i getPosition() const override final;
        void setPosition(vec2i pos) override final;
        void setForeground(Color color, DefaultColor) override final;
        void setBackground(Color, DefaultColor) override final;

      private:
        SDL_Rect _dims;
        Texture const *_texture;
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_OBJECT_RECTANGLE_HPP_)
