/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SFML_OBJECT_RECTANGLE_HPP_
#define ARCADE_GRAPHICS_SFML_OBJECT_RECTANGLE_HPP_

#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <arcade/Color.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/types.hpp>

namespace sf
{
    class RenderTarget;
}

namespace arcade
{
    class Texture;

    class Rectangle : public IGameObject, public sf::Drawable {
      public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Instantiation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Default constructor.
        Rectangle();

        /// Cannot copy rectangles.
        Rectangle(Rectangle const &) = delete;

        /// Move constructor.
        Rectangle(Rectangle &&);

        /// Assignment move operator.
        Rectangle &operator=(Rectangle &&);

        /// Destructor.
        ~Rectangle() = default;

        static std::unique_ptr<Rectangle> create(Texture const *texture, vec2u size);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IGameObject Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Type getType() const override final;
        vec2u getSize() const override final;
        vec2i getPosition() const override final;
        void setPosition(vec2i pos) override final;
        void setForeground(Color color, DefaultColor) override final;
        void setBackground(Color color, DefaultColor) override final;

      private:
        /// size, in units.
        vec2u _size;
        /// position, in units.
        vec2i _pos;

        sf::RectangleShape _foreground;
        sf::RectangleShape _background;

        Rectangle(sf::RectangleShape &&inner);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Utilities
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void updateSize();
        void updatePosition();

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // sf::Drawable Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SFML_OBJECT_RECTANGLE_HPP_)
