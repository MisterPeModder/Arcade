#include <cstddef>
#include <cstdint>
#include <iostream>

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <arcade/Color.hpp>
#include <arcade/Event.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/graphics/units.hpp>
#include <arcade/types.hpp>

#include "AssetManager.hpp"
#include "Renderer.hpp"
#include "SfmlDisplay.hpp"
#include "event.hpp"

namespace arcade
{
    SfmlDisplay::SfmlDisplay() : _window(), _renderStates()
    {
        // do nothing here, real init is done in ::setup()
    }

    void SfmlDisplay::setup()
    {
        this->_window.create(sf::VideoMode(WINDOW_SIZE_PIXELS.x, WINDOW_SIZE_PIXELS.y), "Arcade (SFML)");
        this->_renderStates = sf::RenderStates(sf::BlendAlpha);
        std::cout << "[sfml]: setup" << std::endl;
    }

    void SfmlDisplay::close()
    {
        std::cout << "[sfml]: close" << std::endl;
        this->_window.close();
    }

    IDisplay::Type SfmlDisplay::getType() const { return IDisplay::Type::Graphical2D; }

    vec2u SfmlDisplay::getSize() const { return this->_size; }

    bool SfmlDisplay::pollEvent(Event &event)
    {
        sf::Event rawEvent;

        if (!this->_window.pollEvent(rawEvent) || !arcade::translateSfmlEvent(rawEvent, event)) {
            if (!this->_window.isOpen()) {
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
        this->_window.clear(sf::Color(std::to_integer<uint8_t>(color.r), std::to_integer<uint8_t>(color.g),
            std::to_integer<uint8_t>(color.b), 255 - std::to_integer<uint8_t>(color.a)));
    }

    void SfmlDisplay::render(std::function<void(IRenderer &)> drawer)
    {
        Renderer renderer(this->_window, this->_renderStates);
        drawer(renderer);
    }

    void SfmlDisplay::display() { this->_window.display(); }

    void SfmlDisplay::loadAssets(std::function<void(IAssetManager &)> loader)
    {
        AssetManager manager;
        loader(manager);
    }

    void SfmlDisplay::updateSize()
    {
        sf::Vector2u size = this->_window.getSize();

        this->_size = toUnits(vec2u{size.x, size.y});
    }
} // namespace arcade
