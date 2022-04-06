/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SFML_ASSET_TEXTURE_HPP_
#define ARCADE_GRAPHICS_SFML_ASSET_TEXTURE_HPP_

#include <filesystem>
#include <memory>

#include <SFML/Graphics/Texture.hpp>

#include <arcade/IAsset.hpp>

namespace arcade
{
    class Texture : public IAsset {
      public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Instantiation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Default constructor.
        Texture();

        /// Cannot copy textures.
        Texture(Texture const &) = delete;

        /// Move constructor.
        Texture(Texture &&);

        /// Assignment move operator.
        Texture &operator=(Texture &&);

        /// Destructor.
        ~Texture() = default;

        static std::unique_ptr<Texture> fromFile(std::filesystem::path const &path);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IAsset Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Type getType() const override final;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Miscellaneous
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        sf::Texture const &getInner() const;

      private:
        sf::Texture _inner;

        Texture(sf::Texture &&inner);
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SFML_ASSET_TEXTURE_HPP_)
