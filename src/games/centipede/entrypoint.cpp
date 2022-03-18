#include <iostream>
#include <memory>

#include <arcade/Event.hpp>
#include <arcade/IAsset.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/IGame.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    class Centipede : public IGame {
      public:
        Centipede() : _logoTexture(nullptr), _logo(nullptr), _dragging(false)
        {
            // ...
        }

        void setup(IDisplay &display) override final
        {
            this->_display = &display;
            this->_logoTexture = display.loadAsset("./doc/logo.png", IAsset::Type::Texture);
            this->_logo = display.createRectObject({200, 200}, this->_logoTexture.get());
            this->_dragging = false;
            this->_state = State::Loaded;
        }

        void setDisplay(IDisplay &display) override final
        {
            this->_display = &display;
        }

        void close() override final
        {
            this->_logo.reset();
            this->_logoTexture.reset();
        }

        void setState(State state) override final
        {
            this->_state = state;
        }

        State getState() const override final
        {
            return this->_state;
        }

        unsigned int getScore() const override final
        {
            return -42;
        }

        void update(float delta) override final
        {
            (void)delta;
            // ...
        }

        void draw() override final
        {
            this->_display->drawGameObject(*this->_logo);
        }

        void handleEvent(Event &event) override final
        {
            if (event.type == Event::EventType::Resized
                || (event.type == Event::EventType::KeyPressed && event.key.code == 'c')) {
                vec2u size(this->_logo->getSize());
                vec2u winSize = this->_display->getSize();
                vec2i pos{static_cast<int>(winSize.x) / 2 - static_cast<int>(size.x) / 2,
                    static_cast<int>(winSize.y) / 2 - static_cast<int>(size.y) / 2};

                this->_logo->setPosition(pos);
                this->_dragging = false;
            } else if (event.type == Event::EventType::MouseButtonPressed) {
                this->_dragging = true;
            } else if (event.type == Event::EventType::MouseButtonReleased) {
                this->_dragging = false;
            } else if (event.type == Event::EventType::MouseMoved && this->_dragging) {
                vec2u size(this->_logo->getSize());
                vec2i pos{
                    event.mouseMove.x - static_cast<int>(size.x) / 2, event.mouseMove.y - static_cast<int>(size.y) / 2};

                this->_logo->setPosition(pos);
            }
        }

      private:
        State _state;
        IDisplay *_display;

        std::unique_ptr<IAsset> _logoTexture;
        std::unique_ptr<IGameObject> _logo;

        bool _dragging;
    };

    static arcade::IGame *GAME_INSTANCE = nullptr;

    ARCADE_GAME_ENTRY_POINT
    {
        std::cout << "[centipede]: called entry point" << std::endl;
        return GAME_INSTANCE;
    }

    [[gnu::constructor]] void onConstruct()
    {
        GAME_INSTANCE = new Centipede();
        std::cout << "[centipede]: constructed" << std::endl;
    }

    [[gnu::destructor]] void onDestroy()
    {
        delete GAME_INSTANCE;
        GAME_INSTANCE = nullptr;
        std::cout << "[centipede]: destroyed" << std::endl;
    }
} // namespace arcade
