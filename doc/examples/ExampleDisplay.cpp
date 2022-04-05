#include <functional>
#include <iostream>

#include <arcade/Color.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    struct Event;
    class IAsset;
    class IAssetManager;
    class IGameObject;
    class IRenderer;
} // namespace arcade

// Using 'using' imports to keep the example clean, do not use in final code!
using ::arcade::Color;
using ::arcade::DefaultColor;
using ::arcade::Event;
using ::arcade::IAsset;
using ::arcade::IAssetManager;
using ::arcade::IDisplay;
using ::arcade::IGameObject;
using ::arcade::IRenderer;
using ::arcade::vec2u;

class ExampleDisplay : public IDisplay {
  public:
    ExampleDisplay()
    {
        // ...
    }

    void setup() override final
    {
        // ...
    }

    void close() override final
    {
        // ...
    }

    Type getType() const override final
    {
        return Type::Graphical2D;
    }

    vec2u getSize() const override final
    {
        return {0, 0};
    }

    virtual bool pollEvent(Event &event) override final
    {
        (void)event;
        return false;
    }

    void clear(Color color, DefaultColor backup) override final
    {
        (void)color;
        (void)backup;
    }

    void render(std::function<void(IRenderer &)> drawer) override final
    {
        (void)drawer;
    }

    void display() override final
    {
        // ...
    }

    void loadAssets(std::function<void(IAssetManager &)> loader) override final
    {
        (void)loader;
    }
};
/// end_display_impl

static IDisplay *DISPLAY_INSTANCE = nullptr;

ARCADE_DISPLAY_ENTRY_POINT
{
    std::cout << "[example display]: called entry point" << std::endl;
    return DISPLAY_INSTANCE;
}

[[gnu::constructor]] void onConstruct()
{
    DISPLAY_INSTANCE = new ExampleDisplay();
    std::cout << "[example display]: constructed" << std::endl;
}

[[gnu::destructor]] void onDestroy()
{
    delete DISPLAY_INSTANCE;
    DISPLAY_INSTANCE = nullptr;
    std::cout << "[example display]: destroyed" << std::endl;
}
