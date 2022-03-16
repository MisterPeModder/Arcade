#include <arcade/IGame.hpp>
#include <iostream>

namespace arcade
{
    class IDisplay;
    struct Event;

    class Centipede : public IGame {
      public:
        Centipede()
        {
            // ...
        }

        void setup(IDisplay &display) override final
        {
            this->_state = State::Loaded;
            this->_display = &display;
        }

        void setDisplay(IDisplay &display) override final
        {
            this->_display = &display;
        }

        void close() override final
        {
            // ...
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
            // ...
        }

        void handleEvent(Event &event) override final
        {
            (void)event;
            // ...
        }

      private:
        State _state;
        IDisplay *_display;
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
