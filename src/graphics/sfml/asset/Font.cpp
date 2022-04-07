#include <utility>

#include <arcade/IAsset.hpp>

#include "Font.hpp"

namespace arcade
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Instantiation
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Font::Font() : _inner() {}

    Font::Font(sf::Font &&inner) : _inner(inner) {}

    Font::Font(Font &&other) : _inner(std::move(other._inner)) {}

    Font &Font::operator=(Font &&other)
    {
        this->_inner = std::move(other._inner);
        return *this;
    }

    std::unique_ptr<Font> Font::fromFile(std::filesystem::path const &path)
    {
        sf::Font inner;

        if (!inner.loadFromFile(path.string()))
            return std::unique_ptr<Font>();
        return std::unique_ptr<Font>(new Font(std::move(inner)));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // IAsset Implementation
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    IAsset::Type Font::getType() const { return Type::Font; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Miscellaneous
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::Font const &Font::getInner() const { return this->_inner; }
} // namespace arcade
