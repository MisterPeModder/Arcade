#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include <arcade/Color.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/graphics/units.hpp>
#include <arcade/types.hpp>

#include "../asset/Font.hpp"
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
        : _renderer(renderer), _sprite(sprite), _foregroundColor(Color::Transparent),
          _backgroundColor(Color::Transparent)
    {
        vec2i scaled = toPixels(static_cast<vec2i>(vec2u{size.x, size.y}));
        this->_dims = {0, 0, scaled.x, scaled.y};
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

        // Draw background, if present
        if (this->_backgroundColor.toInteger() != Color::Transparent.toInteger())
            this->drawColor(this->_backgroundColor);

        // Draw foreground, if present
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

    std::unique_ptr<Rectangle> Rectangle::create(SDL_Renderer *renderer, Font const *font, std::string_view text)
    {
        if (!font)
            throw std::logic_error("Missing font");
        SDL_Texture *sprite(font->render(renderer, text));

        if (!sprite)
            return std::unique_ptr<Rectangle>();
        vec2i textureSize = {0, 0};

        SDL_QueryTexture(sprite, nullptr, nullptr, &textureSize.x, &textureSize.y);

        vec2u size = toUnits(static_cast<vec2u>(textureSize));
        return std::unique_ptr<Rectangle>(new Rectangle(renderer, sprite, size));
    }

    IGameObject::Type Rectangle::getType() const
    {
        return IGameObject::Type::Rect;
    }

    vec2u Rectangle::getSize() const
    {
        return toUnits(static_cast<vec2u>(vec2i{this->_dims.w, this->_dims.h}));
    }

    vec2i Rectangle::getPosition() const
    {
        return toUnits(vec2i{this->_dims.x, this->_dims.y});
    }

    void Rectangle::setPosition(vec2i pos)
    {
        vec2i scaled = toPixels(pos);
        this->_dims.x = scaled.x;
        this->_dims.y = scaled.y;
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
