/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SDL2_ASSET_TEXTURE_HPP_
#define ARCADE_GRAPHICS_SDL2_ASSET_TEXTURE_HPP_

#include <filesystem>
#include <memory>

#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_surface.h>

#include <arcade/Color.hpp>
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

        void render(SDL_Rect dims) const;

        void tint(Color color);

        static std::unique_ptr<Texture> fromFile(std::filesystem::path const &path, SDL_Renderer *renderer);

      private:
        SDL_Surface *_surface;
        SDL_Texture *_texture;
        SDL_Renderer *_renderer;

        /// Creates a texture from a SDL2 surface.
        Texture(SDL_Surface *surface, SDL_Texture *texture, SDL_Renderer *renderer);
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_ASSET_TEXTURE_HPP_)
