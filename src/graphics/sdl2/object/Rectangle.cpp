#include <cstddef>
#include <cstdint>

#include <arcade/Color.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/types.hpp>

#include "../asset/Texture.hpp"
#include "Rectangle.hpp"

namespace arcade
{
    Rectangle::Rectangle()
        : _dims({0, 0, 0, 0}), _renderer(nullptr), _sprite(nullptr), _foregroundColor(Color::Transparent),
          _backgroundColor(Color::Transparent)
    {
    }

    Rectangle::Rectangle(SDL_Renderer *renderer, SDL_Texture *sprite, vec2u size)
        : _dims({0, 0, static_cast<int>(size.x), static_cast<int>(size.y)}), _renderer(renderer), _sprite(sprite),
          _foregroundColor(Color::Transparent), _backgroundColor(Color::Transparent)
    {
    }

    Rectangle::Rectangle(Rectangle &&other)
        : _dims(other._dims), _renderer(other._renderer), _sprite(other._sprite),
          _foregroundColor(other._foregroundColor), _backgroundColor(other._backgroundColor)
    {
        other._dims = {0, 0, 0, 0};
        other._renderer = nullptr;
        other._sprite = nullptr;
        other._foregroundColor = Color::Transparent;
        other._backgroundColor = Color::Transparent;
    }

    Rectangle &Rectangle::operator=(Rectangle &&other)
    {
        this->_dims = other._dims;
        this->_renderer = other._renderer;
        this->_sprite = other._sprite;
        this->_foregroundColor = other._foregroundColor;
        this->_backgroundColor = other._backgroundColor;
        other._dims = {0, 0, 0, 0};
        other._renderer = nullptr;
        other._sprite = nullptr;
        other._foregroundColor = Color::Transparent;
        other._backgroundColor = Color::Transparent;
        return *this;
    }

    Rectangle::~Rectangle()
    {
        if (this->_sprite)
            SDL_DestroyTexture(this->_sprite);
        this->_dims = {0, 0, 0, 0};
        this->_renderer = nullptr;
        this->_sprite = nullptr;
        this->_backgroundColor = 0;
    }

    void Rectangle::draw() const
    {
        if (!this->_renderer) [[unlikely]]
            return; // just in case...
        Color color(this->_backgroundColor);

        // Draw background, if present
        if (this->_backgroundColor.toInteger() != Color::Transparent.toInteger())
            this->drawColor(this->_backgroundColor);

        // Draw forground, if present
        if (this->_sprite)
            SDL_RenderCopy(this->_renderer, this->_sprite, nullptr, &this->_dims);
        else
            this->drawColor(this->_foregroundColor);
    }

    void Rectangle::drawColor(Color color) const
    {
        SDL_SetRenderDrawColor(this->_renderer, std::to_integer<uint8_t>(color.r), std::to_integer<uint8_t>(color.g),
            std::to_integer<uint8_t>(color.b), 255 - std::to_integer<uint8_t>(color.a));
        SDL_RenderFillRect(this->_renderer, &this->_dims);
        SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 0);
    }

    std::unique_ptr<Rectangle> Rectangle::create(SDL_Renderer *renderer, Texture const *texture, vec2u size)
    {
        if (texture) {
            SDL_Texture *sprite(texture->createSprite(renderer));

            if (sprite)
                return std::unique_ptr<Rectangle>(new Rectangle(renderer, sprite, size));
        }
        return std::unique_ptr<Rectangle>(new Rectangle(renderer, nullptr, size));
    }

    IGameObject::Type Rectangle::getType() const
    {
        return IGameObject::Type::Rect;
    }

    vec2u Rectangle::getSize() const
    {
        return {static_cast<unsigned int>(this->_dims.w), static_cast<unsigned int>(this->_dims.h)};
    }

    vec2i Rectangle::getPosition() const
    {
        return {this->_dims.x, this->_dims.y};
    }

    void Rectangle::setPosition(vec2i pos)
    {
        this->_dims.x = pos.x;
        this->_dims.y = pos.y;
    }

    void Rectangle::setForeground(Color color, DefaultColor)
    {
        this->_foregroundColor = color;
        if (this->_sprite) {
            SDL_SetTextureColorMod(this->_sprite, std::to_integer<uint8_t>(color.r), std::to_integer<uint8_t>(color.g),
                std::to_integer<uint8_t>(color.b));
            SDL_SetTextureAlphaMod(this->_sprite, 255 - std::to_integer<uint8_t>(color.a));
        }
    }

    void Rectangle::setBackground(Color color, DefaultColor)
    {
        this->_backgroundColor = color;
    }
} // namespace arcade
