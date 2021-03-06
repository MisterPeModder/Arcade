/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SFML_OBJECT_TEXT_HPP_
#define ARCADE_GRAPHICS_SFML_OBJECT_TEXT_HPP_

#include <memory>
#include <string_view>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Text.hpp>

#include <arcade/Color.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/IMutableText.hpp>
#include <arcade/types.hpp>

namespace sf
{
    class RenderTarget;
}

namespace arcade
{
    class Font;

    class Text : public IGameObject, public IMutableText, public sf::Drawable {
      public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Instantiation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Default constructor.
        Text();

        /// Cannot copy texts.
        Text(Text const &) = delete;

        /// Move constructor.
        Text(Text &&);

        /// Move assignment operator.
        Text &operator=(Text &&);

        /// Destructor.
        ~Text() = default;

        static std::unique_ptr<Text> create(Font const *font, std::string_view text);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IGameObject Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Type getType() const override final;
        vec2u getSize() const override final;
        vec2i getPosition() const override final;
        void setPosition(vec2i pos) override final;
        void setForeground(Color color, DefaultColor) override final;
        void setBackground(Color color, DefaultColor) override final;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IMutableText Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void setText(std::string_view text) override final;

      private:
        /// size, in units.
        vec2u _size;
        /// position, in units.
        vec2i _pos;

        sf::Text _inner;
        sf::RectangleShape _background;

        Text(sf::Text &&inner);

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

#endif // !defined(ARCADE_GRAPHICS_SFML_OBJECT_TEXT_HPP_)
