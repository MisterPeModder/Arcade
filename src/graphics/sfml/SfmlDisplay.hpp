/// @file
/// @internal
///
/// SFML Graphics Backend.

#ifndef ARCADE_GRAPHICS_SFML_SFML_DISPLAY_HPP_
#define ARCADE_GRAPHICS_SFML_SFML_DISPLAY_HPP_

#include <memory>
#include <stdexcept>
#include <string_view>

#include <arcade/IAsset.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/types.hpp>

namespace sf
{
    class Window;
}

namespace arcade
{
    class IGameObject;
    struct Event;

    /// SFML Graphics implementation.
    class SfmlDisplay : public IDisplay {
      public:
        SfmlDisplay();

        void setup() override final;

        void close() override final;

        Type getType() const override final;

        std::unique_ptr<IAsset> loadAsset(std::string_view name, IAsset::Type type) override final;

        vec2u getSize() const override final;

        bool pollEvent(Event &event) override final;

        void clear(Color color, DefaultColor backup) override final;

        void render() override final;

        void drawGameObject(const IGameObject &object) override final;

        std::unique_ptr<IGameObject> createTextObject(std::string_view text, IAsset const *font) const override final;

        std::unique_ptr<IGameObject> createRectObject(vec2u size, IAsset const *texture) const override final;

      private:
        std::unique_ptr<sf::Window> _window;
        vec2u _size;

        void updateSize();
    };
} // namespace arcade

#endif //! defined(ARCADE_GRAPHICS_SFML_SFML_DISPLAY_HPP_)
