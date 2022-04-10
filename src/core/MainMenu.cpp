#include "util/LibrarySelector.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <span>

#include <arcade/Event.hpp>
#include <arcade/IAsset.hpp>
#include <arcade/IAssetManager.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/IMutableText.hpp>
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

    MainMenu::MainMenu()
        : _state(State::Ended), _playerName({'?', '?', '?', '\0'}), _playerNamePos({0, 0}), _editingName(false),
          _editCursor(0)
    {
    }

    MainMenu::MainMenu(LibrarySelector<IDisplay> &displays, LibrarySelector<IGame> &games, Scoreboard &scoreboard)
        : _state(State::Ended), _playerName({'?', '?', '?', '\0'}), _displays(&displays), _games(&games),
          _scoreboard(&scoreboard), _playerNamePos({0, 0}), _editingName(false), _editCursor(0)
    {
    }

    MainMenu::MainMenu(MainMenu &&other)
        : _state(other._state), _playerName(other._playerName), _displays(std::move(other._displays)),
          _games(std::move(other._games)), _scoreboard(other._scoreboard), _font(std::move(other._font)),
          _objects(std::move(other._objects)), _playerNameText(std::move(other._playerNameText)),
          _playerNamePos(other._playerNamePos), _editingName(other._editingName), _editCursor(other._editCursor),
          _clickHandlers(other._clickHandlers)
    {
    }

    MainMenu &MainMenu::operator=(MainMenu &&other)
    {
        this->_state = other._state;
        this->_playerName = other._playerName;
        this->_displays = std::move(other._displays);
        this->_games = std::move(other._games);
        this->_scoreboard = std::move(other._scoreboard);
        this->_font = std::move(other._font);
        this->_objects = std::move(other._objects);
        this->_playerNameText = std::move(other._playerNameText);
        this->_playerNamePos = other._playerNamePos;
        this->_editingName = other._editingName;
        this->_editCursor = other._editCursor;
        this->_clickHandlers = std::move(other._clickHandlers);
        return *this;
    }

    std::string_view MainMenu::getPlayerName() const { return std::string_view(this->_playerName.data(), 3); }

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
        IGameObjectPtr title(this->stringToText(manager, Color::White, DefaultColor::White, "[MAIN MENU]", {1, 0}));

        auto convert(std::bind(
            &MainMenu::stringToText, this, std::cref(manager), Color::Black, DefaultColor::Black, _1, vec2i{0, 0}));
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
        unsigned int gamesWidth((gameNameObjects.empty() ? 5 : std::ranges::max(gameNameObjects | objectWidth)) + 3);
        unsigned int displaysWidth(
            (displayNameObjects.empty() ? 5 : std::ranges::max(displayNameObjects | objectWidth)) + 3);

        background->setPosition({0, 0});
        background->setForeground(Color(0x2b2d42), DefaultColor::Black);

        gamesBox = manager.createRectObject({gamesWidth, height});
        displaysBox = manager.createRectObject({displaysWidth, height});

        gamesBox->setPosition({1, 2});
        displaysBox->setPosition({static_cast<int>(gamesBox->getSize().x) + 2, 2});
        quitBox->setPosition({1, 2 + static_cast<int>(height) + 1});

        vec2i scoresPos{displaysBox->getPosition().x + static_cast<int>(displaysBox->getSize().x) + 1, 2};
        this->_scoreboard->initDisplay(manager, this->_font.get(), scoresPos + 1, height + 2);
        scoresBox = manager.createRectObject(this->_scoreboard->getSize() + vec2u{3, 2});
        scoresBox->setPosition(scoresPos);

        gamesBox->setForeground(Color(0xeae2b7), DefaultColor::White);
        displaysBox->setForeground(Color(0xf77f00), DefaultColor::White);
        quitBox->setForeground(Color(0xd62828), DefaultColor::Red);
        scoresBox->setForeground(Color(0x2a6f97), DefaultColor::White);

        moveInBounds(*gamesBox, gameNameObjects);
        moveInBounds(*displaysBox, displayNameObjects);

        vec2u quitBoxSize(quitBox->getSize());

        nameBox = manager.createRectObject(
            {static_cast<unsigned>(scoresBox->getPosition().x) - quitBoxSize.x - 3, quitBoxSize.y});
        nameBox->setPosition(quitBox->getPosition() + vec2i{static_cast<int>(quitBoxSize.x + 1), 0});
        nameBox->setForeground(Color(0x90be6d), DefaultColor::White);

        quitText->setPosition(quitBox->getPosition() + 1);
        this->_playerNamePos = nameBox->getPosition() + 1;

        char buf[11];
        this->writePlayerName(buf);
        this->_playerNameText = this->stringToText(manager, Color::Black, DefaultColor::Black, buf);
        this->updatePlayerNameText();

        int bottom(quitBox->getPosition().y + quitBoxSize.y);

        // Initialize the click handlers
        this->initClickHandlers(quitBox.get(), nameBox.get(), gameNameObjects, displayNameObjects);

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
        this->_objects.push_back(std::move(quitText));
        this->_objects.push_back(std::move(title));

        std::array<std::string_view, 5> hints{
            "<s+J>/<s+L>: previous/next display",
            "<a+J>/<a+L>: previous/next game",
            "<(a+)ESC>: (force) quit game",
            "<ENTER>: edit name",
            "<c-R>: restart game",
        };

        this->stringsToText(manager, Color(0x999999), DefaultColor::White, {1, bottom + 1}, this->_objects, hints);
    }

    void MainMenu::close()
    {
        this->_playerNameText.reset();
        this->_clickHandlers.clear();
        this->_objects.clear();
        this->_scoreboard->closeDisplay();
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
        this->_scoreboard->render(renderer);
        renderer.draw(*this->_playerNameText);
    }

    void MainMenu::handleEvent(Event &event)
    {
        if (event.type == Event::Type::MouseButtonReleased) {
            using handlerType = decltype(this->_clickHandlers)::value_type;
            auto found(std::ranges::find_if(
                this->_clickHandlers, std::bind(isInBounds, _1, event.mouseButton.pos), &handlerType::first));

            if (found != this->_clickHandlers.end())
                found->second(*this); // execute the corresponding function if found
        } else if (event.type == Event::Type::KeyReleased) {
            if (event.key.code == '\n') {
                this->toggleEditingName();
            } else if (this->_editingName && std::isalnum(event.key.code)) {
                if (this->_editCursor > 2)
                    this->_editCursor = 0;
                this->_playerName[this->_editCursor++] = std::toupper(event.key.code);
                this->updatePlayerNameText();
            }
        }
    }

    void MainMenu::initClickHandlers(IGameObject *quitBox, IGameObject *nameBox,
        std::span<IGameObjectPtr> gameNameObjects, std::span<IGameObjectPtr> displayNameObjects)
    {
        this->_clickHandlers.clear();
        this->_clickHandlers.emplace_back(quitBox, std::bind(&MainMenu::setState, _1, State::Ended));
        this->_clickHandlers.emplace_back(nameBox, &MainMenu::toggleEditingName);

        for (auto begin = gameNameObjects.begin(), end = gameNameObjects.end(), it = begin; it != end; ++it) {
            size_t index(static_cast<size_t>(it - begin));
            this->_clickHandlers.emplace_back(it->get(), [=](MainMenu &menu) { menu._games->select(index); });
        }
        for (auto begin = displayNameObjects.begin(), end = displayNameObjects.end(), it = begin; it != end; ++it) {
            size_t index(static_cast<size_t>(it - begin));
            this->_clickHandlers.emplace_back(it->get(), [=](MainMenu &menu) { menu._displays->select(index); });
        }
    }

    void MainMenu::toggleEditingName()
    {
        this->_editingName = !this->_editingName;
        this->_editCursor = 0;
        this->updatePlayerNameText();
    }

    void MainMenu::updatePlayerNameText()
    {
        IMutableText *mutableText = dynamic_cast<IMutableText *>(this->_playerNameText.get());

        if (mutableText) {
            char buf[11];

            this->writePlayerName(buf);
            mutableText->setText(buf);
        }

        Color color(this->_editingName ? Color::Red : Color::Black);
        DefaultColor defaultColor(this->_editingName ? DefaultColor::Red : DefaultColor::Black);

        this->_playerNameText->setForeground(color, defaultColor);
        this->_playerNameText->setPosition(this->_playerNamePos);
    }

    void MainMenu::writePlayerName(char *buf)
    {
        std::ranges::copy("name: ---\0", buf);
        buf[6] = this->_playerName[0];
        buf[7] = this->_playerName[1];
        buf[8] = this->_playerName[2];
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Utilities
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    IGameObjectPtr MainMenu::stringToText(
        IAssetManager const &manager, Color color, DefaultColor defaultColor, std::string_view str, vec2i pos)
    {
        auto text = manager.createTextObject(str, this->_font.get());

        text->setForeground(color, defaultColor);
        text->setBackground(Color::Transparent, DefaultColor::Transparent);
        text->setPosition(pos);
        return text;
    }

    void MainMenu::stringsToText(IAssetManager &manager, Color color, DefaultColor defaultColor, vec2i pos,
        std::vector<IGameObjectPtr> &out, std::span<std::string_view> lines)
    {
        for (std::string_view line : lines) {
            IGameObjectPtr text(this->stringToText(manager, color, defaultColor, line, pos));

            ++pos.y;
            out.push_back(std::move(text));
        }
    }
} // namespace arcade
