#include <iostream>
#include <memory>

#include <arcade/Color.hpp>
#include <arcade/Event.hpp>
#include <arcade/IAsset.hpp>
#include <arcade/IAssetManager.hpp>
#include <arcade/IGame.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/IRenderer.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    class Centipede : public IGame {
      public:
        Centipede() : _logoTexture(nullptr), _logo(nullptr), _dragging(false)
        {
            // ...
        }

        void setup() override final
        {
        }

        void loadAssets(IAssetManager &manager) override final
        {
            this->_logoTexture = manager.loadAsset("./doc/logo.png", IAsset::Type::Texture);
            this->_font = manager.loadAsset("./assets/fonts/roboto_regular.ttf", IAsset::Type::Font);
            this->_logo = manager.createRectObject({27, 27}, this->_logoTexture.get());
            this->_title = manager.createTextObject("This is a test", this->_font.get());
            this->_dragging = false;
            this->_state = State::Loaded;

            this->_logo->setBackground(Color(0x221111));
            this->_logo->setForeground(Color(0xccccff));
            this->_title->setForeground(Color(0x2a9d8f));
            this->_title->setBackground(Color::Green);
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

        void render(IRenderer &renderer) override final
        {
            renderer.draw(*this->_logo);
            renderer.draw(*this->_title);
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

            if (event.type == Event::Type::Resized) {
                vec2u size(this->_logo->getSize());

                this->_logo->setPosition(static_cast<vec2i>(event.size.newSize / 2 - size / 2));
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
