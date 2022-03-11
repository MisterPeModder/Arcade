/*
** EPITECH PROJECT, 2022
** Arcade
** File description:
** IGame
*/

#ifndef IGAME_HPP_
#define IGAME_HPP_

namespace arcade
{
    class Event;

    class IGame {
      public:
        enum class State {
            Loaded,
            Running,
            Paused,
            Ended,
        };
        virtual ~IGame() = default;

        virtual void setup() = 0;
        virtual void setState(State state) = 0;
        virtual State getState() const = 0;

        virtual unsigned int getScore() const = 0;

        virtual void update(float delta) = 0;
        virtual void handleEvent(Event &event) = 0;
    };
} // namespace arcade

#endif /* !IGAME_HPP_ */
