/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SDL2_ASSET_TEXTURE_HPP_
#define ARCADE_GRAPHICS_SDL2_ASSET_TEXTURE_HPP_

#include <filesystem>
#include <memory>

#include <SDL_render.h>
#include <SDL_surface.h>

#include <arcade/IAsset.hpp>

namespace arcade
{
    class Texture : public IAsset {
      public:
        Texture();

        Texture(Texture const &) = delete;

        Texture(Texture &&);
        Texture &operator=(Texture &&);

        ~Texture();

        Type getType() const override final;

        /// Instanciates the wrapped SDL surface (unique texture) into a SDL texture (sprite instance).
        ///
        /// @return The newly created sprite, or @c nullptr on failure.
        SDL_Texture *createSprite(SDL_Renderer *renderer) const;

        static std::unique_ptr<Texture> fromFile(std::filesystem::path const &path);

      private:
        SDL_Surface *_surface;

        /// Creates a texture from a SDL2 surface.
        Texture(SDL_Surface *surface);
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_ASSET_TEXTURE_HPP_)
