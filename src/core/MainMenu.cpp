#include "util/LibrarySelector.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <span>

#include <arcade/Event.hpp>
#include <arcade/IAsset.hpp>
#include <arcade/IAssetManager.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/IRenderer.hpp>

#include "MainMenu.hpp"

namespace rv = std::ranges::views; // shorter iterator transformers
using namespace std::placeholders; // for _1, _2, ... in std::bind()

namespace arcade
{
    static void moveInBounds(IGameObject const &container, std::vector<IGameObjectPtr> const &items)
    {
        vec2i pos = container.getPosition() + 1;

        for (IGameObjectPtr const &item : items) {
            item->setPosition(pos);
            ++pos.y;
        }
    }

    static bool isInBounds(IGameObject const *object, vec2i pos)
    {
        if (object == nullptr)
            return false;

        vec2i topLeft(object->getPosition());
        vec2i botRight(topLeft + static_cast<vec2i>(object->getSize()));

        return topLeft.x <= pos.x && pos.x <= botRight.x && topLeft.y <= pos.y && pos.y <= botRight.y;
    }

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
          _font(std::move(other._font)), _objects(std::move(other._objects)), _clickHandlers(other._clickHandlers)
    {
    }

    MainMenu &MainMenu::operator=(MainMenu &&other)
    {
        this->_state = other._state;
        this->_displays = std::move(other._displays);
        this->_games = std::move(other._games);
        this->_font = std::move(other._font);
        this->_objects = std::move(other._objects);
        this->_clickHandlers = std::move(other._clickHandlers);
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // IGame overrides
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MainMenu::setup() { this->_state = State::Loaded; }

    void MainMenu::loadAssets(IAssetManager &manager, vec2u screenSize)
    {
        this->_font = manager.loadAsset("./assets/fonts/prstartk.ttf", IAsset::Type::Font);

        // declare all game objects
        IGameObjectPtr background(manager.createRectObject(screenSize));
        IGameObjectPtr gamesBox;
        IGameObjectPtr displaysBox;
        IGameObjectPtr quitText(this->stringToText(manager, Color::Black, DefaultColor::Black, "quit"));
        IGameObjectPtr quitBox(manager.createRectObject(quitText->getSize() + 2));
        IGameObjectPtr scoresBox;
        IGameObjectPtr nameBox;
        std::vector<IGameObjectPtr> gameNameObjects;
        std::vector<IGameObjectPtr> displayNameObjects;
        std::vector<IGameObjectPtr> scores;
        IGameObjectPtr nameText(this->stringToText(manager, Color::Black, DefaultColor::Black, "name: ???"));
        IGameObjectPtr title(this->stringToText(manager, Color::White, DefaultColor::White, "[MAIN MENU]"));

        auto convert(
            std::bind(&MainMenu::stringToText, this, std::ref(manager), Color::Black, DefaultColor::Black, _1));
        auto convertWithPrefix([&, this](auto const &str) {
            const char *prefix(*this->_displays && this->_displays->getSelected()->name() == str ? "*" : " ");

            return convert(prefix + str);
        });

        // insert all the game and display names as text objects in their respective lists
        std::ranges::move(rv::keys(*this->_games) | rv::transform(convert), std::back_inserter(gameNameObjects));
        std::ranges::move(
            rv::keys(*this->_displays) | rv::transform(convertWithPrefix), std::back_inserter(displayNameObjects));

        auto getX([](vec2u const &v) { return v.x; });
        auto objectWidth(rv::transform(&IGameObject::getSize) | rv::transform(getX));

        // maximum width of game and display names
        unsigned int height(static_cast<unsigned>(std::max(gameNameObjects.size(), displayNameObjects.size())) + 2);
        unsigned int gamesWidth(std::ranges::max(gameNameObjects | objectWidth) + 3);
        unsigned int displaysWidth(std::ranges::max(displayNameObjects | objectWidth) + 3);
        unsigned int scoresWidth(manager.createTextObject("--- n/a", this->_font.get())->getSize().x + 2);

        background->setPosition({0, 0});
        background->setForeground(Color(0x2b2d42), DefaultColor::Black);

        gamesBox = manager.createRectObject({gamesWidth, height});
        displaysBox = manager.createRectObject({displaysWidth, height});
        scoresBox = manager.createRectObject({scoresWidth, height + 4});

        gamesBox->setPosition({1, 2});
        displaysBox->setPosition({static_cast<int>(gamesBox->getSize().x) + 2, 2});
        quitBox->setPosition({1, 2 + static_cast<int>(height) + 1});
        scoresBox->setPosition({displaysBox->getPosition().x + static_cast<int>(displaysBox->getSize().x) + 1, 2});

        gamesBox->setForeground(Color(0xeae2b7), DefaultColor::White);
        displaysBox->setForeground(Color(0xf77f00), DefaultColor::White);
        quitBox->setForeground(Color(0xd62828), DefaultColor::Red);
        scoresBox->setForeground(Color(0x2a6f97), DefaultColor::White);

        for (unsigned i(0), count(scoresBox->getSize().y - 2); i < count; ++i)
            scores.push_back(this->stringToText(manager, Color::Black, DefaultColor::Black, "--- n/a"));

        moveInBounds(*gamesBox, gameNameObjects);
        moveInBounds(*displaysBox, displayNameObjects);
        moveInBounds(*scoresBox, scores);

        vec2u quitBoxSize(quitBox->getSize());

        nameBox = manager.createRectObject(
            {static_cast<unsigned>(scoresBox->getPosition().x) - quitBoxSize.x - 3, quitBoxSize.y});
        nameBox->setPosition(quitBox->getPosition() + vec2i{static_cast<int>(quitBoxSize.x + 1), 0});
        nameBox->setForeground(Color(0x90be6d), DefaultColor::White);

        quitText->setPosition(quitBox->getPosition() + 1);
        title->setPosition({1, 0});
        nameText->setPosition(nameBox->getPosition() + 1);

        int bottom(quitBox->getPosition().y + quitBoxSize.y);

        // Initialize the click handlers
        this->initClickHandlers(quitBox.get(), gameNameObjects, displayNameObjects);

        // Populate the list of game object from background to foreground
        this->_objects.clear();
        this->_objects.push_back(std::move(background));
        this->_objects.push_back(std::move(gamesBox));
        this->_objects.push_back(std::move(displaysBox));
        this->_objects.push_back(std::move(quitBox));
        this->_objects.push_back(std::move(scoresBox));
        this->_objects.push_back(std::move(nameBox));
        std::ranges::move(gameNameObjects, std::back_inserter(this->_objects));
        std::ranges::move(displayNameObjects, std::back_inserter(this->_objects));
        std::ranges::move(scores, std::back_inserter(this->_objects));
        this->_objects.push_back(std::move(quitText));
        this->_objects.push_back(std::move(nameText));
        this->_objects.push_back(std::move(title));

        std::array<std::string_view, 4> hints{
            "<s+J>/<s+L>: previous/next display",
            "<a+J>/<a+L>: previous/next game",
            "<(a+)ESC>: (force) quit game",
            "<c-R>: restart game",
        };

        this->stringsToText(manager, Color(0x999999), DefaultColor::White, {1, bottom + 1}, this->_objects, hints);
    }

    void MainMenu::close()
    {
        this->_clickHandlers.clear();
        this->_objects.clear();
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

    void MainMenu::render(IRenderer &renderer)
    {
        for (IGameObjectPtr const &object : this->_objects)
            renderer.draw(*object);
    }

    void MainMenu::handleEvent(Event &event)
    {
        if (event.type == Event::Type::MouseButtonReleased) {
            using handlerType = decltype(this->_clickHandlers)::value_type;
            auto found(std::ranges::find_if(
                this->_clickHandlers, std::bind(isInBounds, _1, event.mouseButton.pos), &handlerType::first));

            if (found != this->_clickHandlers.end())
                found->second(*this); // execute the corresponding function if found
        }
    }

    void MainMenu::initClickHandlers(
        IGameObject *quitBox, std::span<IGameObjectPtr> gameNameObjects, std::span<IGameObjectPtr> displayNameObjects)
    {
        this->_clickHandlers.clear();
        this->_clickHandlers.emplace_back(quitBox, std::bind(&MainMenu::setState, _1, State::Ended));

        for (auto begin = gameNameObjects.begin(), end = gameNameObjects.end(), it = begin; it != end; ++it) {
            size_t index(static_cast<size_t>(it - begin));
            this->_clickHandlers.emplace_back(it->get(), [=](MainMenu &menu) { menu._games->select(index); });
        }
        for (auto begin = displayNameObjects.begin(), end = displayNameObjects.end(), it = begin; it != end; ++it) {
            size_t index(static_cast<size_t>(it - begin));
            this->_clickHandlers.emplace_back(it->get(), [=](MainMenu &menu) { menu._displays->select(index); });
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Utilities
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    IGameObjectPtr MainMenu::stringToText(
        IAssetManager &manager, Color color, DefaultColor defaultColor, std::string_view str)
    {
        auto text = manager.createTextObject(str, this->_font.get());

        text->setForeground(color, defaultColor);
        text->setBackground(Color::Transparent, DefaultColor::Transparent);
        return text;
    }

    void MainMenu::stringsToText(IAssetManager &manager, Color color, DefaultColor defaultColor, vec2i pos,
        std::vector<IGameObjectPtr> &out, std::span<std::string_view> lines)
    {
        for (std::string_view line : lines) {
            IGameObjectPtr text(this->stringToText(manager, color, defaultColor, line));

            text->setPosition(pos);
            ++pos.y;
            out.push_back(std::move(text));
        }
    }
} // namespace arcade
