#include <filesystem>
#include <stdexcept>

#include <arcade/IAsset.hpp>
#include <arcade/types.hpp>

#include "AssetManager.hpp"
#include "asset/Font.hpp"
#include "asset/Texture.hpp"
#include "object/Rectangle.hpp"

namespace arcade
{
    class IGameObject;

    AssetManager::AssetManager(SDL_Renderer *renderer) : _renderer(renderer)
    {
    }

    AssetManager::AssetManager(AssetManager &&other) : _renderer(other._renderer)
    {
        other._renderer = nullptr;
    }

    AssetManager &AssetManager::operator=(AssetManager &&other)
    {
        this->_renderer = other._renderer;
        other._renderer = nullptr;
        return *this;
    }

    IDisplay::Type AssetManager::getType() const
    {
        return IDisplay::Type::Graphical2D;
    }

    std::unique_ptr<IAsset> AssetManager::loadAsset(std::string_view name, IAsset::Type type)
    {
        std::filesystem::path path(name);

        switch (type) {
            case IAsset::Type::Texture: return Texture::fromFile(path);
            case IAsset::Type::Font: return Font::fromFile(path);
            default: return std::unique_ptr<IAsset>();
        }
    }

    std::unique_ptr<IGameObject> AssetManager::createTextObject(std::string_view text, IAsset const *font) const
    {
        Font const *f = dynamic_cast<Font const *>(font);

        if (font != nullptr && f == nullptr)
            throw std::logic_error("textObject asset must be of font type");
        return Rectangle::create(this->_renderer, f, text);
    }

    std::unique_ptr<IGameObject> AssetManager::createRectObject(vec2u size, IAsset const *texture) const
    {
        Texture const *t = dynamic_cast<Texture const *>(texture);

        if (texture != nullptr && t == nullptr)
            throw std::logic_error("rectObject asset must be of texture type");
        return Rectangle::create(this->_renderer, t, size);
    }

} // namespace arcade
