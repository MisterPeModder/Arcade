/// @file
/// @internal
///
/// SFML Graphics Backend.

#ifndef ARCADE_GRAPHICS_SFML_SFML_DISPLAY_HPP_
#define ARCADE_GRAPHICS_SFML_SFML_DISPLAY_HPP_

#include <functional>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <arcade/Color.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    class IAssetManager;
    class IRenderer;
    struct Event;

    /// SFML Graphics implementation.
    class SfmlDisplay : public IDisplay {
      public:
        SfmlDisplay();

        void setup() override final;

        void close() override final;

        Type getType() const override final;

        vec2u getSize() const override final;

        bool pollEvent(Event &event) override final;

        void clear(Color color, DefaultColor backup) override final;

        void render(std::function<void(IRenderer &)> drawer) override final;

        void display() override final;

        void loadAssets(std::function<void(IAssetManager &)> loader) override final;

      private:
        sf::RenderWindow _window;
        vec2u _size;
        sf::RenderStates _renderStates;

        void updateSize();
    };
} // namespace arcade

#endif //! defined(ARCADE_GRAPHICS_SFML_SFML_DISPLAY_HPP_)
