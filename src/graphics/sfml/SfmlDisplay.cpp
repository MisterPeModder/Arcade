#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <arcade/Event.hpp>
#include <arcade/IAsset.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/graphics/units.hpp>
#include <arcade/types.hpp>

#include "SfmlDisplay.hpp"
#include "event.hpp"

namespace arcade
{
    SfmlDisplay::SfmlDisplay() : _window()
    {
        // do nothing here, real init is done in ::setup()
    }

    void SfmlDisplay::setup()
    {
        this->_window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(WINDOW_SIZE_PIXELS.x, WINDOW_SIZE_PIXELS.y), "Arcade (SFML)");
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
        (void)name;
        (void)type;
        return std::unique_ptr<IAsset>();
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
    }

    void SfmlDisplay::drawGameObject(IGameObject const &object)
    {
        (void)object;
    }

    std::unique_ptr<IGameObject> SfmlDisplay::createTextObject(std::string_view text, IAsset const *font) const
    {
        (void)text;
        (void)font;
        return std::unique_ptr<IGameObject>();
    }

    std::unique_ptr<IGameObject> SfmlDisplay::createRectObject(vec2u size, IAsset const *texture) const
    {
        (void)size;
        (void)texture;
        return std::unique_ptr<IGameObject>();
    }

    void SfmlDisplay::updateSize()
    {
        sf::Vector2u size = this->_window->getSize();

        this->_size = toUnits(vec2u{size.x, size.y});
    }
} // namespace arcade
