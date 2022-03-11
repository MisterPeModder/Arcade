#include <arcade/IDisplay.hpp>
#include <arcade/IGameObject.hpp>
#include <iostream>

extern "C"
{
    arcade::IDisplay *ARCADE_DISPLAY_INSTANCE = nullptr;

    arcade::IDisplay *arcade_DisplayEntryPoint(void)
    {
        std::cout << "[sdl2]: called entry point" << std::endl;
        return ARCADE_DISPLAY_INSTANCE;
    }
}

namespace arcade
{

    class Sdl2Display : public IDisplay {
      public:
        static IDisplay *INSTANCE;

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

        Type getDisplayType() const override final
        {
            return Type::Graphical2D;
        };

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

        std::unique_ptr<IGameObject> createTextObject(std::string_view text, IAsset const &font) const override final
        {
            (void)text;
            (void)font;
            return std::unique_ptr<IGameObject>(nullptr);
        }

        std::unique_ptr<IGameObject> createRectObject(vec2u size, IAsset const &texture) const override final
        {
            (void)size;
            (void)texture;
            return std::unique_ptr<IGameObject>(nullptr);
        }
    };

    [[gnu::constructor]] void onConstruct()
    {
        ARCADE_DISPLAY_INSTANCE = new Sdl2Display();
        std::cout << "[sdl2]: constructed" << std::endl;
    }

    [[gnu::destructor]] void onDestroy()
    {
        delete ARCADE_DISPLAY_INSTANCE;
        ARCADE_DISPLAY_INSTANCE = nullptr;
        std::cout << "[sdl2]: destroyed" << std::endl;
    }
} // namespace arcade
