#include <iostream>
#include <memory>
#include <string_view>

#include <arcade/IAsset.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    struct Event;
}

namespace arcade
{
    class Sdl2Display : public IDisplay {
      public:
        Sdl2Display()
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

    static IDisplay *DISPLAY_INSTANCE = nullptr;

    ARCADE_DISPLAY_ENTRY_POINT
    {
        std::cout << "[sdl2]: called entry point" << std::endl;
        return DISPLAY_INSTANCE;
    }

    [[gnu::constructor]] void onConstruct()
    {
        DISPLAY_INSTANCE = new Sdl2Display();
        std::cout << "[sdl2]: constructed" << std::endl;
    }

    [[gnu::destructor]] void onDestroy()
    {
        delete DISPLAY_INSTANCE;
        DISPLAY_INSTANCE = nullptr;
        std::cout << "[sdl2]: destroyed" << std::endl;
    }
} // namespace arcade
