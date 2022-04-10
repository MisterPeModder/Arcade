/// @file
/// @internal
///
/// The main menu of Arcade, implented as an IGame

#ifndef ARCADE_CORE_MAIN_MENU_HPP_
#define ARCADE_CORE_MAIN_MENU_HPP_

#include <memory>
#include <span>
#include <string>
#include <vector>

#include <arcade/IGame.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/aliases.hpp>

#include "util/LibrarySelector.hpp"

namespace arcade
{
    class IAsset;

    /// IGame implementation used for rendering the main menu.
    /// This is not part of a dynamic library.
    class MainMenu : public IGame {
      public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Initialization
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Default constructor.
        MainMenu();
        explicit MainMenu(LibrarySelector<IDisplay> &displays, LibrarySelector<IGame> &games);

        /// Cannot copy main menu.
        MainMenu(MainMenu const &) = delete;
        /// Move constructor.
        MainMenu(MainMenu &&);
        /// Assignment move operator.
        MainMenu &operator=(MainMenu &&);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IGame overrides
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void setup() override final;
        void loadAssets(IAssetManager &manager, vec2u displaySize) override final;
        void close() override final;
        void setState(State state) override final;
        State getState() const override final;
        unsigned int getScore() const override final;
        void update(float delta) override final;
        void render(IRenderer &renderer) override final;
        void handleEvent(Event &event) override final;

      private:
        State _state;

        LibrarySelector<IDisplay> *_displays;
        LibrarySelector<IGame> *_games;

        IAssetPtr _font;

        std::vector<IGameObjectPtr> _objects;

        std::vector<std::pair<IGameObject *, std::function<void(MainMenu &)>>> _clickHandlers;

        void initClickHandlers(IGameObject *quitBox, std::span<IGameObjectPtr> gameNameObjects,
            std::span<IGameObjectPtr> displayNameObjects);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Utilities
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        IGameObjectPtr stringToText(
            IAssetManager &manager, Color color, DefaultColor defaultColor, std::string_view line);

        void stringsToText(IAssetManager &manager, Color color, DefaultColor defaultColor, vec2i pos,
            std::vector<IGameObjectPtr> &out, std::span<std::string_view> lines);
    };
} // namespace arcade

#endif // !defined(ARCADE_CORE_MAIN_MENU_HPP_)
