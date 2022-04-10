#include "LibrarySelector.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <ratio>
#include <thread> // IWYU pragma: keep
#include <utility>
#include <vector>
#include <unordered_map>

#include <arcade/Color.hpp>
#include <arcade/Event.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/IGame.hpp>

#include "Core.hpp"
#include "LibraryInstance.hpp"
#include "util/DynamicLibrary.hpp"

// IWYU pragma: no_include <bits/this_thread_sleep.h>

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
        for (auto &[path, lib] : libs) {
            std::string name(prettyLibName(path));

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

        this->_mainMenu = MainMenu(this->_displays, this->_games);

        this->setDisplayInstance(startingDisplay);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Main Loop
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Core::run()
    {
        this->_lastUpdate = this->_clock.now();
        this->_lastFrame = this->_clock.now();

        do {
            this->updateDisplaySelection();
            this->updateGameSelection();
            if (!this->forwardEvents())
                break;
            if (!this->updateGame())
                continue;
            this->render();
            this->awaitNextFrame();
        } while (this->_mainMenu.getState() != IGame::State::Ended);
    }

    bool Core::forwardEvents()
    {
        Event event;

        // Poll all event and forward each to the current game (if any)
        while (this->_display->pollEvent(event)) {
            // Exit the current game if the escape key was pressed
            if (event.type == Event::Type::KeyReleased) {
                if (event.key.code == '\x1b') {
                    this->_game->setState(IGame::State::Ended);

                    // if 'alt' is pressed close the whole program
                    if (event.key.alt) {
                        std::cout << "Force quitting..." << std::endl;
                        this->_mainMenu.setState(IGame::State::Ended);
                        return false;
                    }
                    break;
                } else if (event.key.code == 'j' || event.key.code == 'J') {
                    if (event.key.shift)
                        this->_displays.selectPrevious();
                    else if (event.key.alt)
                        this->_games.selectPrevious();
                } else if (event.key.code == 'l' || event.key.code == 'L') {
                    if (event.key.shift)
                        this->_displays.selectNext();
                    else if (event.key.alt)
                        this->_games.selectNext();
                }
            }

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
            std::cout << "Game ended, player " << this->_mainMenu.getPlayerName() << " scored "
                      << this->_game->getScore() << " points" << std::endl;
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

        if (selectedDisplay == this->_displays.cend() || selectedDisplay->instance() == this->_display.get())
            return;

        IGame *gameInstance = this->_game.clear();

        this->_display.set(selectedDisplay->instance());
        this->_game.set(gameInstance);
        this->reloadAssets();
    }

    void Core::updateGameSelection()
    {
        auto selectedGame(this->_games.getSelected());

        if (selectedGame == this->_games.cend()) {
            if (this->_game.get() == &this->_mainMenu)
                return;
            if (this->_game.get() != nullptr)
                std::cout << "Exiting to main menu" << std::endl;
            this->_game.set(&this->_mainMenu); // returning to main menu
        } else if (selectedGame->instance() != this->_game.get()) {
            std::cout << "Loading " << selectedGame->name() << std::endl;
            this->_game.set(selectedGame->instance()); // game to game change
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

    void Core::setDisplayInstance(IDisplay *display)
    {
        std::cout << "\nLoading graphics..." << std::endl;

        auto found = std::ranges::find(this->_displays, display, &LibraryEntry<IDisplay>::instance);
        this->_displays.select(found);

        this->_display.set(display);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Miscellaneous
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string Core::prettyLibName(std::string_view path)
    {
        std::string name(std::filesystem::path(path).filename().stem());

        // remove the "arcade_" prefix if it is present.
        if (name.starts_with("arcade_"))
            name.replace(0, 7, "");
        if (name.empty())
            name = "<unamed>";
        else
            *name.data() = std::toupper(*name.data()); // uppercase the first letter
        return name;
    }
} // namespace arcade
