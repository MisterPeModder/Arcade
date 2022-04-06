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
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Instantiation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Default constructor.
        Font();

        /// Cannot copy fonts.
        Font(Font const &) = delete;

        /// Move constructor.
        Font(Font &&);

        /// Assignment move operator.
        Font &operator=(Font &&);

        /// Destructor.
        ~Font();

        static std::unique_ptr<Font> fromFile(std::filesystem::path const &path);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IAsset Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Type getType() const override final;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Miscellaneous
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        SDL_Texture *render(SDL_Renderer *renderer, std::string_view text) const;

      private:
        TTF_Font *_font;

        Font(TTF_Font *font);
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_ASSET_FONT_HPP_)
