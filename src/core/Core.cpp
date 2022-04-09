#include <chrono>
#include <iostream>
#include <thread>

#include <arcade/Event.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/IGame.hpp>

#include "Core.hpp"
#include "util/DynamicLibrary.hpp"

namespace arcade
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Instantiation
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Core::Core(DynamicLibrary::Registry &libs, IDisplay *startingDisplay)
    {
        LibrarySelector<IDisplay>::Builder displaysBuilder;
        LibrarySelector<IGame>::Builder gamesBuilder;

        // Convert our generic libs into either game or graphics (or both)
        for (auto &[name, lib] : libs) {
            try {
                displaysBuilder.add(name, lib.symbol<IDisplay::EntryPoint>(IDisplay::ENTRY_POINT)());
            } catch (DynamicLibrary::UnknownSymbolError &) {
            }

            try {
                gamesBuilder.add(name, lib.symbol<IGame::EntryPoint>(IGame::ENTRY_POINT)());
            } catch (DynamicLibrary::UnknownSymbolError &) {
            }
        }

        this->_displays = displaysBuilder.build();
        this->_games = gamesBuilder.build();

        this->_mainMenu = std::move(MainMenu(this->_displays, this->_games));

        std::cout << "\nLoading graphics..." << std::endl;
        this->_display.set(startingDisplay);

        auto x = this->_displays.getSelected();
        auto y = this->_displays.end();

        std::cout << "display selected: " << x->second << std::endl;
        std::cout << "display end: " << y->second << std::endl;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Main Loop
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Core::run()
    {
        this->_lastUpdate = this->_clock.now();
        this->_lastFrame = this->_clock.now();

        for (;;) {
            this->updateDisplaySelection();
            this->updateGameSelection();
            if (!this->forwardEvents())
                break;
            if (!this->updateGame())
                continue;
            this->render();
            this->awaitNextFrame();
        }
    }

    bool Core::forwardEvents()
    {
        Event event;

        // Poll all event and forward each to the current game (if any)
        while (this->_display->pollEvent(event)) {
            this->_game->handleEvent(event);

            if (event.type == Event::Type::Closed)
                return false;
        }
        return true;
    }

    bool Core::updateGame()
    {
        IGame::State state(this->_game->getState());

        if (state == IGame::State::Ended && this->_games.getSelected() != this->_games.cend()) {
            std::cout << "Game ended, final score: " << this->_game->getScore() << std::endl;
            this->_games.select(this->_games.end());
            return false;
        } else if (state == IGame::State::Running) {
            std::chrono::duration<double> elapsed(this->_clock.now() - this->_lastUpdate);

            this->_game->update(elapsed.count());
            this->_lastUpdate = this->_clock.now();
        }
        return true;
    }

    void Core::render()
    {
        this->_display->clear(Color::Black, DefaultColor::Black);
        this->_display->render([&](auto &renderer) { this->_game->render(renderer); });
        this->_display->display();
    }

    void Core::awaitNextFrame()
    {
        using namespace std::chrono_literals;

        constexpr auto frameTime(1s / static_cast<double>(FRAMERATE_LIMIT)); // the duration of a frame
        std::chrono::duration<double> elapsed(this->_clock.now() - this->_lastFrame);

        std::this_thread::sleep_for(frameTime - elapsed);
        this->_lastFrame = this->_clock.now();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // State Update
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Core::updateDisplaySelection()
    {
        auto selectedDisplay(this->_displays.getSelected());

        if (selectedDisplay == this->_displays.cend() || selectedDisplay->second == this->_display.get())
            return;

        this->_display.set(selectedDisplay->second);
        this->reloadAssets();
    }

    void Core::updateGameSelection()
    {
        auto selectedGame(this->_games.getSelected());

        if (selectedGame == this->_games.cend()) {
            if (this->_game.get() != &this->_mainMenu) {
                if (this->_game.get() != nullptr)
                    std::cout << "Exiting to main menu" << std::endl;
                this->_game.set(&this->_mainMenu); // returning to main menu
            }
        } else if (selectedGame->second != this->_game.get()) {
            std::cout << "Loading " << selectedGame->first << std::endl;
            this->_game.set(selectedGame->second); // game to game change
        } else {
            return;
        }

        this->reloadAssets();
        this->_game->setState(IGame::State::Running);
    }

    void Core::reloadAssets()
    {
        this->_display->loadAssets([&](auto &manager) { this->_game->loadAssets(manager, this->_display->getSize()); });
    }
} // namespace arcade
