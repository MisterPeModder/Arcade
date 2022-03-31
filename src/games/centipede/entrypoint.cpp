#include <iostream>
#include <memory>

#include <arcade/Color.hpp>
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
            this->_font = display.loadAsset("./assets/fonts/roboto_regular.ttf", IAsset::Type::Font);
            this->_logo = display.createRectObject({27, 27}, this->_logoTexture.get());
            this->_title = display.createTextObject("This is a test", this->_font.get());
            this->_dragging = false;
            this->_state = State::Loaded;

            this->_logo->setBackground(Color(0x221111));
            this->_logo->setForeground(Color(0xccccff));
            this->_title->setForeground(Color(0x2a9d8f));
            this->_title->setBackground(Color::Green);
        }

        void setDisplay(IDisplay &display) override final
        {
            this->_display = &display;
        }

        void close() override final
        {
            this->_logo.reset();
            this->_logoTexture.reset();
            this->_font.reset();
            this->_title.reset();
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
            this->_display->drawGameObject(*this->_title);
        }

        void handleEvent(Event &event) override final
        {
            if (event.type == Event::Type::KeyReleased) {
                if (event.key.code == '\x1b')
                    this->_state = State::Ended;
                else if (event.key.code == 'p')
                    this->_state = this->_state == State::Paused ? State::Running : State::Paused;
            }

            // Don't update anything else if the game is paused
            if (this->_state == State::Paused)
                return;

            if (event.type == Event::Type::Resized
                || (event.type == Event::Type::KeyPressed && event.key.code == 'c')) {
                vec2u size(this->_logo->getSize());
                vec2u winSize = this->_display->getSize();
                vec2i pos{static_cast<int>(winSize.x) / 2 - static_cast<int>(size.x) / 2,
                    static_cast<int>(winSize.y) / 2 - static_cast<int>(size.y) / 2};

                this->_logo->setPosition(pos);
                this->_dragging = false;
            } else if (event.type == Event::Type::MouseButtonPressed) {
                this->_dragging = true;
            } else if (event.type == Event::Type::MouseButtonReleased) {
                std::cout << "released at " << event.mouseButton.pos.x << ", " << event.mouseButton.pos.y << std::endl;
                this->_dragging = false;
            } else if (event.type == Event::Type::MouseMoved && this->_dragging) {
                vec2u size(this->_logo->getSize());
                vec2i mousePos(event.mouseMove.pos);
                vec2i pos{mousePos.x - static_cast<int>(size.x) / 2, mousePos.y - static_cast<int>(size.y) / 2};

                this->_logo->setPosition(pos);
            }
        }

      private:
        State _state;
        IDisplay *_display;

        std::unique_ptr<IAsset> _logoTexture;
        std::unique_ptr<IAsset> _font;
        std::unique_ptr<IGameObject> _logo;
        std::unique_ptr<IGameObject> _title;

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
