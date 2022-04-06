#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <utility>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include <arcade/Color.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/graphics/units.hpp>
#include <arcade/types.hpp>

#include "../asset/Texture.hpp"
#include "Rectangle.hpp"

namespace arcade
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Instantiation
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Rectangle::Rectangle() : _size{0, 0}, _pos{0, 0}, _foreground() {}

    Rectangle::Rectangle(sf::RectangleShape &&inner) : _foreground(inner), _background()
    {
        this->updatePosition();
        this->updateSize();
    }

    Rectangle::Rectangle(Rectangle &&other)
        : _foreground(std::move(other._foreground)), _background(std::move(other._background))
    {
        this->updatePosition();
        this->updateSize();
    }

    Rectangle &Rectangle::operator=(Rectangle &&other)
    {
        this->_foreground = std::move(other._foreground);
        this->_background = std::move(other._background);
        this->updatePosition();
        this->updateSize();
        return *this;
    }

    std::unique_ptr<Rectangle> Rectangle::create(Texture const *texture, vec2u size)
    {
        vec2<float> scaledSize(toPixels(static_cast<vec2<float>>(size)));
        sf::RectangleShape inner(sf::Vector2f{scaledSize.x, scaledSize.y});

        if (texture)
            inner.setTexture(&texture->getInner(), true);
        return std::unique_ptr<Rectangle>(new Rectangle(std::move(inner)));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // IGameObject Implementation
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    IGameObject::Type Rectangle::getType() const { return Type::Rect; }

    vec2u Rectangle::getSize() const { return this->_size; }

    vec2i Rectangle::getPosition() const { return this->_pos; }

    void Rectangle::setPosition(vec2i pos)
    {
        vec2i scaledPos = toPixels(pos);

        this->_foreground.setPosition(sf::Vector2f(scaledPos.x, scaledPos.y));
        this->updatePosition();
    }

    void Rectangle::setForeground(Color color, DefaultColor)
    {
        sf::Color c(std::to_integer<uint8_t>(color.r), std::to_integer<uint8_t>(color.g),
            std::to_integer<uint8_t>(color.b), 255 - std::to_integer<uint8_t>(color.a));

        this->_foreground.setFillColor(c);
        this->_foreground.setOutlineColor(c);
    }

    void Rectangle::setBackground(Color color, DefaultColor)
    {
        sf::Color c(std::to_integer<uint8_t>(color.r), std::to_integer<uint8_t>(color.g),
            std::to_integer<uint8_t>(color.b), 255 - std::to_integer<uint8_t>(color.a));

        this->_background.setFillColor(c);
        this->_background.setOutlineColor(c);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Utilities
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Rectangle::updateSize()
    {
        sf::FloatRect bounds(this->_foreground.getGlobalBounds());
        sf::Vector2f size{bounds.width, bounds.height};

        this->_background.setSize(size);
        this->_size = static_cast<vec2u>(toUnits(vec2<float>{std::max(0.0f, size.x), std::max(0.0f, size.y)}));
    }

    void Rectangle::updatePosition()
    {
        sf::Vector2f const &pos(this->_foreground.getPosition());

        this->_background.setPosition(pos);
        this->_pos = static_cast<vec2i>(toUnits(vec2<float>{pos.x, pos.y}));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // sf::Drawable Implementation
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Rectangle::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(this->_background, states);
        target.draw(this->_foreground, states);
    }
} // namespace arcade
