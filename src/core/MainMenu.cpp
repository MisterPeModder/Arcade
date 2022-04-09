#include <iostream>

#include <arcade/Event.hpp>
#include <arcade/IAsset.hpp>
#include <arcade/IAssetManager.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/IRenderer.hpp>

#include "MainMenu.hpp"

namespace arcade
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    MainMenu::MainMenu() : _state(State::Ended) {}

    MainMenu::MainMenu(LibrarySelector<IDisplay> &displays, LibrarySelector<IGame> &games)
        : _state(State::Ended), _displays(&displays), _games(&games)
    {
    }

    MainMenu::MainMenu(MainMenu &&other)
        : _state(other._state), _displays(std::move(other._displays)), _games(std::move(other._games)),
          _font(std::move(other._font)), _title(std::move(other._title))
    {
    }

    MainMenu &MainMenu::operator=(MainMenu &&other)
    {
        this->_state = other._state;
        this->_displays = std::move(other._displays);
        this->_games = std::move(other._games);
        this->_font = std::move(other._font);
        this->_title = std::move(other._title);
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // IGame overrides
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MainMenu::setup() { this->_state = State::Loaded; }

    void MainMenu::loadAssets(IAssetManager &manager, vec2u)
    {
        this->_font = manager.loadAsset("./assets/fonts/roboto_regular.ttf", IAsset::Type::Font);
        this->_title = manager.createTextObject("Arcade Main Menu", this->_font.get());

        this->_title->setForeground(Color::White);
        this->_title->setBackground(Color::Black);
    }

    void MainMenu::close()
    {
        std::cout << "MainMenu::close()" << std::endl;
        this->_title.reset();
        this->_font.reset();
    }

    void MainMenu::setState(State state)
    {
        // the main menu cannot be paused
        if (state != State::Paused)
            this->_state = state;
    }

    IGame::State MainMenu::getState() const { return this->_state; }

    unsigned int MainMenu::getScore() const
    {
        // score is irrelevant for the main menu
        return 42;
    }

    void MainMenu::update(float)
    {
        // no logic to update
    }

    void MainMenu::render(IRenderer &renderer) { renderer.draw(*this->_title); }

    void MainMenu::handleEvent(Event &event)
    {
        if (event.type == Event::Type::KeyReleased) {
            if (event.key.code == '\x1b')
                this->_state = State::Ended;
            if (event.key.code == 'a')
                this->_games->select(this->_games->begin());
        }
    }
} // namespace arcade
