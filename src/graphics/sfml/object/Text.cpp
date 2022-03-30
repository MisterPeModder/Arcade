#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>

#include <arcade/Color.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/graphics/units.hpp>
#include <arcade/types.hpp>

#include "../asset/Font.hpp"
#include "Text.hpp"

namespace arcade
{
    Text::Text() : _size{0, 0}, _pos{0, 0}, _inner(), _background()
    {
    }

    Text::Text(sf::Text &&inner) : _inner(inner), _background()
    {
        this->_background.setFillColor(sf::Color::Transparent);
        this->updatePosition();
        this->updateSize();
    }

    Text::Text(Text &&other) : _inner(std::move(other._inner)), _background(std::move(other._background))
    {
        this->updatePosition();
        this->updateSize();
    }

    Text &Text::operator=(Text &&other)
    {
        this->_inner = std::move(other._inner);
        this->_background = std::move(other._background);
        this->updatePosition();
        this->updateSize();
        return *this;
    }

    std::unique_ptr<Text> Text::create(Font const *font, std::string_view text)
    {
        if (!font)
            throw std::logic_error("Missing font");

        sf::Text inner(sf::String(std::string(text)), font->getInner());

        inner.setCharacterSize(PIXELS_PER_UNIT);
        std::cout << "char size: " << inner.getCharacterSize() << std::endl;
        inner.setScale({0.9, 0.9});

        return std::unique_ptr<Text>(new Text(std::move(inner)));
    }

    IGameObject::Type Text::getType() const
    {
        return Type::Text;
    }

    vec2u Text::getSize() const
    {
        return this->_size;
    }

    void Text::updateSize()
    {
        sf::FloatRect bounds(this->_inner.getGlobalBounds());
        sf::Vector2f size{bounds.width, PIXELS_PER_UNIT};

        this->_background.setSize(size);
        this->_size = static_cast<vec2u>(toUnits(vec2<float>{std::max(0.0f, size.x), std::max(0.0f, size.y)}));
    }

    vec2i Text::getPosition() const
    {
        return this->_pos;
    }

    void Text::setPosition(vec2i pos)
    {
        vec2i scaledPos = toPixels(pos);

        this->_inner.setPosition(sf::Vector2f(scaledPos.x, scaledPos.y));
        this->updatePosition();
    }

    void Text::updatePosition()
    {
        sf::Vector2f const &pos(this->_inner.getPosition());

        this->_background.setPosition(pos);
        this->_pos = static_cast<vec2i>(toUnits(vec2<float>{pos.x, pos.y}));
    }

    void Text::setForeground(Color color, DefaultColor)
    {
        sf::Color c(std::to_integer<uint8_t>(color.r), std::to_integer<uint8_t>(color.g),
            std::to_integer<uint8_t>(color.b), 255 - std::to_integer<uint8_t>(color.a));

        this->_inner.setFillColor(c);
        this->_inner.setOutlineColor(c);
    }

    void Text::setBackground(Color color, DefaultColor)
    {
        this->_background.setFillColor(sf::Color(std::to_integer<uint8_t>(color.r), std::to_integer<uint8_t>(color.g),
            std::to_integer<uint8_t>(color.b), 255 - std::to_integer<uint8_t>(color.a)));
    }

    void Text::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(this->_background, states);
        target.draw(this->_inner, states);
    }
} // namespace arcade
