#include <utility>

#include <arcade/IAsset.hpp>

#include "Texture.hpp"

namespace arcade
{
    Texture::Texture() : _inner() {}

    Texture::Texture(sf::Texture &&inner) : _inner(inner) {}

    Texture::Texture(Texture &&other) : _inner(std::move(other._inner)) {}

    Texture &Texture::operator=(Texture &&other)
    {
        this->_inner = std::move(other._inner);
        return *this;
    }

    IAsset::Type Texture::getType() const { return IAsset::Type::Texture; }

    sf::Texture const &Texture::getInner() const { return this->_inner; }

    std::unique_ptr<Texture> Texture::fromFile(std::filesystem::path const &path)
    {
        sf::Texture inner;

        if (!inner.loadFromFile(path.string()))
            return std::unique_ptr<Texture>();
        return std::unique_ptr<Texture>(new Texture(std::move(inner)));
    }
} // namespace arcade
