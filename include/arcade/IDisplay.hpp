/*
** EPITECH PROJECT, 2022
** Arcade
** File description:
** IDisplay
*/

#ifndef IDISPLAY_HPP_
#define IDISPLAY_HPP_

#include <memory>
#include "types.hpp"
#include <string_view>

namespace arcade
{
    class IGameObject;
    class Event;
    class IAsset;

    class IDisplay {
      public:
        enum class AssetType {
            Font,
            Texture,
            CharSet,
        };
        enum class DisplayType {
            Terminal,
            Graphical2D,
        };
        virtual ~IDisplay() = default;

        virtual void setup() = 0;
        virtual void close() = 0;
        virtual DisplayType getDisplayType() const = 0;

        virtual std::shared_ptr<IAsset> loadAsset(std::string_view name, std::string_view asset, AssetType type) = 0;

        virtual vec2u getSize() const = 0;

        virtual bool pollEvent(Event &event) const = 0;
        virtual void draw() const = 0;
        virtual void drawGameObject(const IGameObject &object) const = 0;

        std::unique_ptr<IGameObject> createTextObject(std::string_view text, IAsset *const font);
        std::unique_ptr<IGameObject> createRectObject(vec2u size, IAsset *const texture);
    };
} // namespace arcade

#endif /* !IDISPLAY_HPP_ */
