/*
** EPITECH PROJECT, 2022
** Arcade
** File description:
** IGameObject
*/

#ifndef IGAMEOBJECT_HPP_
#define IGAMEOBJECT_HPP_

#include "types.hpp"

namespace arcade
{
    class IGameObject {
      public:
        enum ObjectType {
            Text,
            Rect,
        };

        virtual ~IGameObject() = default;

        virtual ObjectType getType() const = 0;
        virtual vec2u getSize() const = 0;
        virtual vec2u getPosition() const = 0;
        virtual void setPosition(vec2u pos) = 0;
    };
} // namespace arcade

#endif /* !IGAMEOBJECT_HPP_ */
