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

    Core::Core(DynamicLibrary::Registry &libs, IDisplay *startingDisplay) : _displays(), _games(), _display(), _game()
    {
        // Convert our generic libs into either game or graphics (or both)
        for (auto &[name, lib] : libs) {
            try {
                this->_displays.emplace(name, lib.symbol<IDisplay::EntryPoint>(IDisplay::ENTRY_POINT)());
            } catch (DynamicLibrary::UnknownSymbolError &) {
            }

            try {
                this->_games.emplace(name, lib.symbol<IGame::EntryPoint>(IGame::ENTRY_POINT)());
            } catch (DynamicLibrary::UnknownSymbolError &) {
            }
        }

        // Some info output, might remove later
        std::cout << "\nAvailable displays:\n";
        for (auto &[name, _] : this->_displays) {
            std::cout << "- " << name << '\n';
        }

        std::cout << "\nAvailable games:\n";
        for (auto &[name, _] : this->_games) {
            std::cout << "- " << name << '\n';
        }

        std::cout << "\nLoading graphics..." << std::endl;
        this->_display.set(startingDisplay);

        // Load first game available
        auto firstGame = this->_games.begin();

        if (firstGame != this->_games.end()) {
            std::cout << "\nLoading graphics..." << std::endl;
            this->_game.set(firstGame->second);
            this->_display->loadAssets(
                [&](auto &manager) { firstGame->second->loadAssets(manager, this->_display->getSize()); });
        } else {
            std::cout << "\nNo game available!" << std::endl;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Runtime
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Core::eventLoop()
    {
        using namespace std::chrono_literals;

        Event event;
        std::chrono::steady_clock clock;
        auto lastUpdate(clock.now());
        auto frameTime(1s / static_cast<double>(FRAMERATE_LIMIT)); // the duration of a frame

        if (this->_game)
            this->_game->setState(IGame::State::Running);

        // Inform the game about the current screen size
        this->updateGameSize();

        std::cout << "\nRunning..." << std::endl;
        for (;;) {
            // Poll all event and forward each to the current game (if any)
            while (this->_display->pollEvent(event)) {
                if (this->_game)
                    this->_game->handleEvent(event);
                if (event.type == Event::Type::Closed)
                    return;
            }

            this->_display->clear(Color::Black, DefaultColor::Black);

            auto currentUpdate(clock.now());
            std::chrono::duration<double> elapsed(currentUpdate - lastUpdate);

            if (this->_game) {
                IGame::State state(this->_game->getState());

                if (state == IGame::State::Ended) {
                    std::cout << "Game ended, final score: " << this->_game->getScore() << std::endl;
                    return;
                } else if (state == IGame::State::Running) {
                    // Update the game's logic
                    this->_game->update(elapsed.count());
                }

                this->_display->render([&](auto &renderer) { this->_game->render(renderer); });
            }

            this->_display->display();

            // Wait for the next frame if needed
            lastUpdate = currentUpdate;
            std::this_thread::sleep_for(frameTime - elapsed);
        }
    }

    void Core::updateGameSize()
    {
        if (!this->_game)
            return;
        Event resizeEvent;

        resizeEvent.type = Event::Type::Resized;
        resizeEvent.size.oldSize = {0, 0};
        resizeEvent.size.newSize = this->_display->getSize();
        this->_game->handleEvent(resizeEvent);
    }
} // namespace arcade
