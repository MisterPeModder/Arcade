#include <arcade/graphics/units.hpp>

#include "../asset/Font.hpp"
#include "Text.hpp"

namespace arcade
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Instantiation
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Text::Text() : Rectangle(), _font(nullptr) {}

    Text::Text(SDL_Renderer *renderer, Font const *font, SDL_Texture *sprite, vec2u size)
        : Rectangle(renderer, sprite, size), _font(font)
    {
    }

    std::unique_ptr<Text> Text::create(SDL_Renderer *renderer, Font const *font, std::string_view text)
    {
        if (!font)
            throw std::logic_error("Missing font");
        SDL_Texture *sprite(font->render(renderer, text));

        if (!sprite)
            return std::unique_ptr<Text>();
        vec2i textureSize = {0, 0};

        SDL_QueryTexture(sprite, nullptr, nullptr, &textureSize.x, &textureSize.y);

        vec2u size = toUnits(static_cast<vec2u>(textureSize));
        return std::unique_ptr<Text>(new Text(renderer, font, sprite, size));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // IGameObject Implementation
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    IGameObject::Type Text::getType() const { return Type::Text; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // IMutableText Implementation
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Text::setText(std::string_view text)
    {
        SDL_Texture *sprite(this->_font->render(this->_renderer, text));

        if (!sprite)
            return;
        vec2i textureSize = {0, 0};

        SDL_QueryTexture(sprite, nullptr, nullptr, &textureSize.x, &textureSize.y);

        if (this->_sprite)
            SDL_DestroyTexture(this->_sprite);
        this->_sprite = sprite;
        this->_dims = {this->_dims.x, this->_dims.y, textureSize.x, textureSize.y};
    }
} // namespace arcade
