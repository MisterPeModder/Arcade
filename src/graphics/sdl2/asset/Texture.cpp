#include <cstddef>
#include <cstdint>

#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>

#include <arcade/Color.hpp>
#include <arcade/IAsset.hpp>

#include "Texture.hpp"

namespace arcade
{
    Texture::Texture() : _surface(nullptr)
    {
    }

    Texture::Texture(SDL_Surface *surface, SDL_Texture *texture, SDL_Renderer *renderer)
        : _surface(surface), _texture(texture), _renderer(renderer)
    {
    }

    Texture::Texture(Texture &&other) : _surface(other._surface), _texture(other._texture), _renderer(other._renderer)
    {
        other._surface = nullptr;
        other._texture = nullptr;
        other._renderer = nullptr;
    }

    Texture &Texture::operator=(Texture &&other)
    {
        if (this->_texture)
            SDL_DestroyTexture(this->_texture);
        if (this->_surface)
            SDL_FreeSurface(this->_surface);
        this->_surface = other._surface;
        this->_texture = other._texture;
        this->_renderer = other._renderer;
        other._surface = nullptr;
        other._texture = nullptr;
        other._renderer = nullptr;
        return *this;
    }

    Texture::~Texture()
    {
        if (this->_texture)
            SDL_DestroyTexture(this->_texture);
        this->_texture = nullptr;
        if (this->_surface)
            SDL_FreeSurface(this->_surface);
        this->_surface = nullptr;
        this->_renderer = nullptr;
    }

    IAsset::Type Texture::getType() const
    {
        return IAsset::Type::Texture;
    }

    void Texture::render(SDL_Rect dims) const
    {
        SDL_RenderCopy(this->_renderer, this->_texture, nullptr, &dims);
    }

    void Texture::tint(Color color)
    {
        SDL_SetTextureColorMod(this->_texture, std::to_integer<uint8_t>(color.r), std::to_integer<uint8_t>(color.g),
            std::to_integer<uint8_t>(color.b));
        SDL_SetTextureAlphaMod(this->_texture, 255 - std::to_integer<uint8_t>(color.a));
    }

    std::unique_ptr<Texture> Texture::fromFile(std::filesystem::path const &path, SDL_Renderer *renderer)
    {
        SDL_Surface *surface(IMG_Load(path.c_str()));

        if (!surface)
            return std::unique_ptr<Texture>();

        SDL_Texture *texture(SDL_CreateTextureFromSurface(renderer, surface));

        if (!texture) {
            SDL_FreeSurface(surface);
            return std::unique_ptr<Texture>();
        }
        return std::unique_ptr<Texture>(new Texture(surface, texture, renderer));
    }
} // namespace arcade
