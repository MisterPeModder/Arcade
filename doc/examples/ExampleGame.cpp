#include <arcade/IGame.hpp>
#include <iostream>

namespace arcade
{
    struct Event;
    class IRenderer;
    class IAssetManager;
} // namespace arcade

// Using 'using' imports to keep the example clean, do not use in final code!
using ::arcade::Event;
using ::arcade::IAssetManager;
using ::arcade::IGame;
using ::arcade::IRenderer;

class ExampleGame : public IGame {
  private:
    State _state;

  public:
    ExampleGame()
    {
        // ...
    }

    void setup() override final
    {
        this->_state = State::Loaded;
    }

    void loadAssets(IAssetManager &manager) override final
    {
        (void)manager;
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

    void render(IRenderer &renderer) override final
    {
        (void)renderer;
    }

    void handleEvent(Event &event) override final
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
