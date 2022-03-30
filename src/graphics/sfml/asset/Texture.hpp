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
        Texture();

        Texture(Texture const &) = delete;

        Texture(Texture &&);
        Texture &operator=(Texture &&);

        ~Texture() = default;

        Type getType() const override final;

        sf::Texture const &getInner() const;

        static std::unique_ptr<Texture> fromFile(std::filesystem::path const &path);

      private:
        sf::Texture _inner;

        Texture(sf::Texture &&inner);
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SFML_ASSET_TEXTURE_HPP_)
