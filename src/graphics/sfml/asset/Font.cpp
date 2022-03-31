#include <utility>

#include <arcade/IAsset.hpp>

#include "Font.hpp"

namespace arcade
{
    Font::Font() : _inner()
    {
    }

    Font::Font(sf::Font &&inner) : _inner(inner)
    {
    }

    Font::Font(Font &&other) : _inner(std::move(other._inner))
    {
    }

    Font &Font::operator=(Font &&other)
    {
        this->_inner = std::move(other._inner);
        return *this;
    }

    IAsset::Type Font::getType() const
    {
        return Type::Font;
    }

    sf::Font const &Font::getInner() const
    {
        return this->_inner;
    }

    std::unique_ptr<Font> Font::fromFile(std::filesystem::path const &path)
    {
        sf::Font inner;

        if (!inner.loadFromFile(path.string()))
            return std::unique_ptr<Font>();
        return std::unique_ptr<Font>(new Font(std::move(inner)));
    }
} // namespace arcade
