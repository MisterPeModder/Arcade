#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>

#include <arcade/IAsset.hpp>

#include "Texture.hpp"

namespace arcade
{
    Texture::Texture() : _surface(nullptr)
    {
    }

    Texture::Texture(SDL_Surface *surface) : _surface(surface)
    {
    }

    Texture::Texture(Texture &&other) : _surface(other._surface)
    {
        other._surface = nullptr;
    }

    Texture &Texture::operator=(Texture &&other)
    {
        if (this->_surface)
            SDL_FreeSurface(this->_surface);
        this->_surface = other._surface;
        other._surface = nullptr;
        return *this;
    }

    Texture::~Texture()
    {
        if (this->_surface)
            SDL_FreeSurface(this->_surface);
        this->_surface = nullptr;
    }

    IAsset::Type Texture::getType() const
    {
        return IAsset::Type::Texture;
    }

    SDL_Texture *Texture::createSprite(SDL_Renderer *renderer) const
    {
        return SDL_CreateTextureFromSurface(renderer, this->_surface);
    }

    std::unique_ptr<Texture> Texture::fromFile(std::filesystem::path const &path)
    {
        SDL_Surface *surface(IMG_Load(path.c_str()));

        if (!surface)
            return std::unique_ptr<Texture>();
        return std::unique_ptr<Texture>(new Texture(surface));
    }
} // namespace arcade
