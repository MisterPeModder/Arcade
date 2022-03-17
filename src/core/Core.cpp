#include <chrono>
#include <iostream>
#include <stdexcept>

#include <arcade/Event.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/IGame.hpp>

#include "Core.hpp"
#include "util/DynamicLibrary.hpp"

namespace arcade
{
    Core::Core(DynamicLibrary::Registry &libs, IDisplay *startingDisplay)
        : _displays(), _games(), _currentDisplay(startingDisplay), _currentGame(nullptr)
    {
        for (auto &[name, lib] : libs) {
            try {
                this->_displays.emplace(name, lib.symbol<IDisplay::EntryPoint>(IDisplay::ENTRY_POINT)());
            } catch (std::runtime_error &) {
            }

            try {
                this->_games.emplace(name, lib.symbol<IGame::EntryPoint>(IGame::ENTRY_POINT)());
            } catch (std::runtime_error &) {
            }
        }

        std::cout << "\nAvailable displays:\n";
        for (auto &[name, _] : this->_displays) {
            std::cout << "- " << name << '\n';
        }

        std::cout << "\nAvailable games:\n";
        for (auto &[name, _] : this->_games) {
            std::cout << "- " << name << '\n';
        }
        std::cout.flush();
    }

    void Core::eventLoop()
    {
        Event event;
        std::chrono::steady_clock clock;
        auto lastUpdate(clock.now());

        std::cout << "\nRunning..." << std::endl;
        for (;;) {
            while (this->_currentDisplay->pollEvent(event)) {
                if (this->_currentGame)
                    this->_currentGame->handleEvent(event);
                if (event.type == Event::EventType::Closed)
                    return;
            }

            if (this->_currentGame) {
                auto currentUpdate(clock.now());
                std::chrono::duration<double> elapsed(currentUpdate - lastUpdate);

                this->_currentGame->update(elapsed.count());
                lastUpdate = currentUpdate;

                this->_currentGame->draw();
            }

            this->_currentDisplay->render();
        }
    }
} // namespace arcade
