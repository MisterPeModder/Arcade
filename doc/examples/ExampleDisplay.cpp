#include <arcade/IDisplay.hpp>
#include <arcade/IGameObject.hpp>
#include <iostream>

// Using 'using' imports to keep the example clean, do not use in final code!
using ::arcade::Event;
using ::arcade::IAsset;
using ::arcade::IDisplay;
using ::arcade::IGameObject;
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

    std::unique_ptr<IAsset> loadAsset(std::string_view name, IAsset::Type type) override final
    {
        (void)name;
        (void)type;
        return std::unique_ptr<IAsset>(nullptr);
    }

    vec2u getSize() const override final
    {
        return {0, 0};
    }

    virtual bool pollEvent(Event &event)
    {
        (void)event;
        return false;
    }

    void render() override final
    {
        // ...
    }

    void drawGameObject(const IGameObject &object) override final
    {
        (void)object;
        // ...
    }

    std::unique_ptr<IGameObject> createTextObject(std::string_view text, IAsset const *font) const override final
    {
        (void)text;
        (void)font;
        return std::unique_ptr<IGameObject>(nullptr);
    }

    std::unique_ptr<IGameObject> createRectObject(vec2u size, IAsset const *texture) const override final
    {
        (void)size;
        (void)texture;
        return std::unique_ptr<IGameObject>(nullptr);
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
