#include <arcade/IGame.hpp>
#include <iostream>

extern "C"
{
    arcade::IGame *ARCADE_GAME_INSTANCE = nullptr;
}

ARCADE_GAME_ENTRY_POINT
{
    std::cout << "[centipede]: called entry point" << std::endl;
    return ARCADE_GAME_INSTANCE;
}

namespace arcade
{
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

        void draw()
        {
            // ...
        }

        void handleEvent(Event &event)
        {
            (void)event;
            // ...
        }

      private:
        State _state;
        IDisplay *_display;
    };

    [[gnu::constructor]] void onConstruct()
    {
        ARCADE_GAME_INSTANCE = new Centipede();
        std::cout << "[centipede]: constructed" << std::endl;
    }

    [[gnu::destructor]] void onDestroy()
    {
        delete ARCADE_GAME_INSTANCE;
        ARCADE_GAME_INSTANCE = nullptr;
        std::cout << "[centipede]: destroyed" << std::endl;
    }
} // namespace arcade
