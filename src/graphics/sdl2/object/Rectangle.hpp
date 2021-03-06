/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SDL2_OBJECT_RECTANGLE_HPP_
#define ARCADE_GRAPHICS_SDL2_OBJECT_RECTANGLE_HPP_

#include <memory>
#include <string_view>

#include <SDL_rect.h>
#include <SDL_render.h>

#include <arcade/Color.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/IMutableText.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    class Font;
    class Texture;

    class Rectangle : public IGameObject {
      public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Instantiation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Rectangle();

        /// Cannot copy rectangle.
        Rectangle(Rectangle const &) = delete;

        /// Move constructor.
        Rectangle(Rectangle &&);

        /// Move assingment operator.
        Rectangle &operator=(Rectangle &&);

        /// Destructor.
        ~Rectangle();

        static std::unique_ptr<Rectangle> create(SDL_Renderer *renderer, Texture const *texture, vec2u size);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Rendering
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void draw() const;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IGameObject Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Type getType() const override;
        vec2u getSize() const override final;
        vec2i getPosition() const override final;
        void setPosition(vec2i pos) override final;
        void setForeground(Color color, DefaultColor) override final;
        void setBackground(Color color, DefaultColor) override final;

      protected:
        /// Position & Size (in pixels, not units)
        SDL_Rect _dims;
        SDL_Renderer *_renderer;
        SDL_Texture *_sprite;

        Color _foregroundColor;
        Color _backgroundColor;

        explicit Rectangle(SDL_Renderer *renderer, SDL_Texture *sprite, vec2u size);

        void drawColor(Color color) const;
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_OBJECT_RECTANGLE_HPP_)
