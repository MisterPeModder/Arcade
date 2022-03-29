#include <SDL_pixels.h>
#include <SDL_surface.h>

#include <arcade/IAsset.hpp>
#include <arcade/graphics/units.hpp>

#include "Font.hpp"

namespace arcade
{
    Font::Font() : _font(nullptr)
    {
    }

    Font::Font(TTF_Font *font) : _font(font)
    {
    }

    Font::Font(Font &&other) : _font(other._font)
    {
        other._font = nullptr;
    }

    Font &Font::operator=(Font &&other)
    {
        this->_font = other._font;
        other._font = nullptr;
        return *this;
    }

    Font::~Font()
    {
        if (this->_font)
            TTF_CloseFont(this->_font);
        this->_font = nullptr;
    }

    IAsset::Type Font::getType() const
    {
        return Type::Font;
    }

    SDL_Texture *Font::render(SDL_Renderer *renderer, std::string_view text) const
    {
        if (!this->_font) [[unlikely]]
            return nullptr;
        SDL_Surface *surface(TTF_RenderUTF8_Blended(this->_font, text.data(), SDL_Color{0xff, 0xff, 0xff, 0xff}));

        if (!surface)
            return nullptr;
        SDL_Texture *texture(SDL_CreateTextureFromSurface(renderer, surface));

        SDL_FreeSurface(surface);
        return texture;
    }

    std::unique_ptr<Font> Font::fromFile(std::filesystem::path const &path)
    {
        TTF_Font *font(TTF_OpenFont(path.c_str(), PIXELS_PER_UNIT));

        if (!font)
            return std::unique_ptr<Font>();
        return std::unique_ptr<Font>(new Font(font));
    }
} // namespace arcade
