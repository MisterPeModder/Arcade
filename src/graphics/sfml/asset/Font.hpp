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
        ~Font() = default;

        static std::unique_ptr<Font> fromFile(std::filesystem::path const &path);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IAsset Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Type getType() const override final;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Miscellaneous
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        sf::Font const &getInner() const;

      private:
        sf::Font _inner;

        Font(sf::Font &&inner);
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SFML_ASSET_FONT_HPP_)
