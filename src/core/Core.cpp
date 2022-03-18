#include <chrono>
#include <iostream>

#include <arcade/Event.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/IGame.hpp>

#include "Core.hpp"
#include "util/DynamicLibrary.hpp"

namespace arcade
{
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
        auto first = this->_games.begin();

        if (first != this->_games.end()) {
            std::cout << "\nLoading graphics..." << std::endl;
            this->_game.set<IDisplay &>(first->second, *this->_display);
        } else {
            std::cout << "\nNo game available!" << std::endl;
        }
    }

    void Core::eventLoop()
    {
        Event event;
        std::chrono::steady_clock clock;
        auto lastUpdate(clock.now());

        std::cout << "\nRunning..." << std::endl;
        for (;;) {
            // Poll all event and forward each to the current game (if any)
            while (this->_display->pollEvent(event)) {
                if (this->_game)
                    this->_game->handleEvent(event);
                if (event.type == Event::EventType::Closed)
                    return;
            }

            if (this->_game) {
                auto currentUpdate(clock.now());
                std::chrono::duration<double> elapsed(currentUpdate - lastUpdate);

                // Update the game's logic
                this->_game->update(elapsed.count());
                lastUpdate = currentUpdate;

                // Render the game
                this->_game->draw();
            }

            // Update the display
            this->_display->render();
        }
    }
} // namespace arcade
