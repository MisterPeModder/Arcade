/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SDL2_ASSET_FONT_HPP_
#define ARCADE_GRAPHICS_SDL2_ASSET_FONT_HPP_

#include <filesystem>
#include <memory>
#include <string_view>

#include <SDL_render.h>
#include <SDL_ttf.h>

#include <arcade/IAsset.hpp>

namespace arcade
{
    class Font : public IAsset {
      public:
        Font();

        Font(Font const &) = delete;

        Font(Font &&);
        Font &operator=(Font &&);

        ~Font();

        Type getType() const override final;

        SDL_Texture *render(SDL_Renderer *renderer, std::string_view text) const;

        static std::unique_ptr<Font> fromFile(std::filesystem::path const &path);

      private:
        TTF_Font *_font;

        Font(TTF_Font *font);
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_ASSET_FONT_HPP_)
