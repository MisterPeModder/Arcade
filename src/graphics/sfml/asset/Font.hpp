/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SFML_ASSET_FONT_HPP_
#define ARCADE_GRAPHICS_SFML_ASSET_FONT_HPP_

#include <filesystem>
#include <memory>

#include <SFML/Graphics/Font.hpp>

#include <arcade/IAsset.hpp>

namespace arcade
{
    class Font : public IAsset {
      public:
        Font();

        Font(Font const &) = delete;

        Font(Font &&);
        Font &operator=(Font &&);

        ~Font() = default;

        Type getType() const override final;

        sf::Font const &getInner() const;

        static std::unique_ptr<Font> fromFile(std::filesystem::path const &path);

      private:
        sf::Font _inner;

        Font(sf::Font &&inner);
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SFML_ASSET_FONT_HPP_)
