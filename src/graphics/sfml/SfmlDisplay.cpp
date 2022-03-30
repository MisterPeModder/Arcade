#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <stdexcept>

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <arcade/Color.hpp>
#include <arcade/Event.hpp>
#include <arcade/IAsset.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/graphics/units.hpp>
#include <arcade/types.hpp>

#include "SfmlDisplay.hpp"
#include "asset/Font.hpp"
#include "asset/Texture.hpp"
#include "event.hpp"
#include "object/Rectangle.hpp"
#include "object/Text.hpp"

namespace arcade
{
    SfmlDisplay::SfmlDisplay() : _window(), _renderStates()
    {
        // do nothing here, real init is done in ::setup()
    }

    void SfmlDisplay::setup()
    {
        this->_window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(WINDOW_SIZE_PIXELS.x, WINDOW_SIZE_PIXELS.y), "Arcade (SFML)");
        this->_renderStates = sf::RenderStates(sf::BlendAlpha);
        std::cout << "[sfml]: setup" << std::endl;
    }

    void SfmlDisplay::close()
    {
        std::cout << "[sfml]: close" << std::endl;
        this->_window.reset();
    }

    IDisplay::Type SfmlDisplay::getType() const
    {
        return IDisplay::Type::Graphical2D;
    }

    std::unique_ptr<IAsset> SfmlDisplay::loadAsset(std::string_view name, IAsset::Type type)
    {
        std::filesystem::path path(name);

        switch (type) {
            case IAsset::Type::Texture: return Texture::fromFile(path);
            case IAsset::Type::Font: return Font::fromFile(path);
            default: return std::unique_ptr<IAsset>();
        }
    }

    vec2u SfmlDisplay::getSize() const
    {
        return this->_size;
    }

    bool SfmlDisplay::pollEvent(Event &event)
    {
        sf::Event rawEvent;

        if (!this->_window->pollEvent(rawEvent) || !arcade::translateSfmlEvent(rawEvent, event)) {
            if (!this->_window->isOpen()) {
                event.type = Event::Type::Closed;
                return true;
            }
            return false;
        }

        // If the window was resized, query the size of the render output and update the event before returning.
        if (event.type == Event::Type::Resized) {
            this->updateSize();
            std::cout << "Resized to " << this->_size.x << "x" << this->_size.y << " units" << std::endl;
        }

        return true;
    }

    void SfmlDisplay::clear(Color color, DefaultColor)
    {
        this->_window->clear(sf::Color(std::to_integer<uint8_t>(color.r), std::to_integer<uint8_t>(color.g),
            std::to_integer<uint8_t>(color.b), 255 - std::to_integer<uint8_t>(color.a)));
    }

    void SfmlDisplay::render()
    {
        this->_window->display();
        this->clear(Color::Black, DefaultColor::Black);
    }

    void SfmlDisplay::drawGameObject(IGameObject const &object)
    {
        sf::Drawable const *drawable = dynamic_cast<sf::Drawable const *>(&object);

        if (drawable)
            this->_window->draw(*drawable, this->_renderStates);
    }

    std::unique_ptr<IGameObject> SfmlDisplay::createTextObject(std::string_view text, IAsset const *font) const
    {
        Font const *f = dynamic_cast<Font const *>(font);

        if (font != nullptr && f == nullptr)
            throw std::logic_error("textObject asset must be of font type");
        return Text::create(f, text);
    }

    std::unique_ptr<IGameObject> SfmlDisplay::createRectObject(vec2u size, IAsset const *texture) const
    {
        Texture const *t = dynamic_cast<Texture const *>(texture);

        if (texture != nullptr && t == nullptr)
            throw std::logic_error("rectObject asset must be of texture type");
        return Rectangle::create(t, size);
    }

    void SfmlDisplay::updateSize()
    {
        sf::Vector2u size = this->_window->getSize();

        this->_size = toUnits(vec2u{size.x, size.y});
    }
} // namespace arcade
