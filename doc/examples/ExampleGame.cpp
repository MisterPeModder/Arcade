#include <arcade/IGame.hpp>
#include <iostream>

// Using 'using' imports to keep the example clean, do not use in final code!
using ::arcade::Event;
using ::arcade::IDisplay;
using ::arcade::IGame;

class ExampleGame : public IGame {
  private:
    State _state;
    IDisplay *_display;

  public:
    ExampleGame()
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

    // [...]

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
};
/// end_game_impl

static IGame *ARCADE_GAME_INSTANCE = nullptr;

ARCADE_GAME_ENTRY_POINT
{
    std::cout << "[example game]: called entry point" << std::endl;
    return ARCADE_GAME_INSTANCE;
}

[[gnu::constructor]] void onConstruct()
{
    ARCADE_GAME_INSTANCE = new ExampleGame();
    std::cout << "[example game]: constructed" << std::endl;
}

[[gnu::destructor]] void onDestroy()
{
    delete ARCADE_GAME_INSTANCE;
    ARCADE_GAME_INSTANCE = nullptr;
    std::cout << "[example game]: destroyed" << std::endl;
}
