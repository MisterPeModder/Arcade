#include <algorithm>
#include <compare>
#include <functional>
#include <memory>
#include <sstream>
#include <utility>

#include <arcade/Color.hpp>
#include <arcade/IAssetManager.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/IMutableText.hpp>
#include <arcade/IRenderer.hpp>
#include <arcade/aliases.hpp>
#include <arcade/types.hpp>

#include "Scoreboard.hpp"

namespace arcade
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Scoreboard Entry
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ScoreboardEntry::ScoreboardEntry() : _gameName(), _playerName{'?', '?', '?', '\0'}, _score(0) {}

    ScoreboardEntry::ScoreboardEntry(std::string_view gameName, std::string_view playerName, unsigned int score)
        : _gameName(gameName), _score(score)
    {
        size_t len(playerName.size());
        this->_playerName = {
            len > 0 ? playerName.data()[0] : '?',
            len > 1 ? playerName.data()[1] : '?',
            len > 2 ? playerName.data()[2] : '?',
            '\0',
        };
    }

    std::string_view ScoreboardEntry::getGameName() const { return this->_gameName; }

    bool ScoreboardEntry::operator==(ScoreboardEntry const &other) const noexcept
    {
        return this->_score == other._score && this->_gameName == this->_gameName
            && this->getPlayerName() == other.getPlayerName();
    }

    auto ScoreboardEntry::operator<=>(ScoreboardEntry const &other) const noexcept
    {
        auto ord1 = this->_score <=> other._score;

        if (ord1 == 0) {
            auto ord2(this->_gameName <=> other._gameName);

            if (ord2 == 0)
                return this->getPlayerName() <=> other.getPlayerName();
            return ord2;
        }
        return ord1;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Initialization
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Scoreboard::Scoreboard() : _size({0, 0}) {}

    void Scoreboard::add(ScoreboardEntry &&entry)
    {
        auto pos(std::ranges::find_if(this->_entries, [&](ScoreboardEntry const &e) { return entry < e; }));

        this->_entries.emplace(pos, entry);
        if (this->_entries.size() > SCOREBOARD_MAX_SIZE)
            this->_entries.resize(SCOREBOARD_MAX_SIZE);
        this->updateDisplay();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Rendering
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scoreboard::initDisplay(IAssetManager &manager, IAsset const *font, vec2i pos, unsigned int maxLines)
    {
        this->_displayLines.clear();

        this->_size = {0, maxLines};

        for (unsigned int i(0); i < maxLines; ++i) {
            IGameObjectPtr line(manager.createTextObject(this->textAt(i), font));

            line->setPosition(pos + vec2i{0, static_cast<int>(i)});
            line->setForeground(Color::Black, DefaultColor::Black);
            this->_size.x = std::max(this->_size.x, line->getSize().x);
            this->_displayLines.emplace_back(std::move(line));
        }
    }

    void Scoreboard::closeDisplay()
    {
        this->_size = {0, 0};
        this->_displayLines.clear();
    }

    void Scoreboard::updateDisplay()
    {
        this->_size.x = 0;

        for (size_t i(0); i < this->_displayLines.size(); ++i) {
            IGameObjectPtr &line(this->_displayLines[i]);
            IMutableText *mutableText(dynamic_cast<IMutableText *>(line.get()));

            if (mutableText)
                mutableText->setText(this->textAt(i));
            this->_size.x = std::max(this->_size.x, line->getSize().x);
        }
    }

    void Scoreboard::render(IRenderer &renderer)
    {
        for (IGameObjectPtr const &line : this->_displayLines)
            renderer.draw(*line);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Utilities
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string Scoreboard::textAt(unsigned int pos) const
    {
        std::ostringstream out;

        out << (pos + 1) << ". ";
        if (static_cast<std::size_t>(pos) >= this->_entries.size()) {
            out << "--- n/a";
        } else {
            ScoreboardEntry const &entry(this->_entries[pos]);

            out << entry.getPlayerName() << ' ' << entry.getScore() << " (" << entry.getGameName() << ")";
        }
        return out.str();
    }
} // namespace arcade
