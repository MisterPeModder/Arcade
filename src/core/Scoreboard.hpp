/// @file
/// @internal

#ifndef ARCADE_CORE_SCOREBOARD_HPP_
#define ARCADE_CORE_SCOREBOARD_HPP_

#include <array>
#include <cstddef>
#include <string>
#include <vector>
#include <string_view>

#include <arcade/aliases.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    class IAsset;
    class IAssetManager;
    class IRenderer;

    constexpr std::size_t SCOREBOARD_MAX_SIZE = 50;

    class ScoreboardEntry {
      public:
        ScoreboardEntry();
        ScoreboardEntry(std::string_view gameName, std::string_view playerName, unsigned int score);

        ScoreboardEntry(ScoreboardEntry const &) = default;
        ScoreboardEntry(ScoreboardEntry &&) = default;

        ScoreboardEntry &operator=(ScoreboardEntry const &) = default;
        ScoreboardEntry &operator=(ScoreboardEntry &&) = default;

        std::string_view getGameName() const;
        constexpr std::string_view getPlayerName() const noexcept
        {
            return std::string_view(this->_playerName.data(), 3);
        };
        constexpr unsigned int getScore() const noexcept { return this->_score; };

        bool operator==(ScoreboardEntry const &other) const noexcept;

        auto operator<=>(ScoreboardEntry const &other) const noexcept;

      private:
        std::string _gameName;
        std::array<char, 4> _playerName;
        unsigned int _score;
    };

    class Scoreboard {
      public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Initialization
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Scoreboard();

        void add(ScoreboardEntry &&entry);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Rendering
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void initDisplay(IAssetManager &manager, IAsset const *font, vec2i pos, unsigned int maxLines);
        void closeDisplay();
        void updateDisplay();

        constexpr vec2u getSize() const noexcept { return this->_size; };

        void render(IRenderer &renderer);

      private:
        std::vector<ScoreboardEntry> _entries;
        std::vector<IGameObjectPtr> _displayLines;

        vec2u _size;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Utilities
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::string textAt(unsigned int pos) const;
    };
} // namespace arcade

#endif // !defined(ARCADE_CORE_SCOREBOARD_HPP_)
