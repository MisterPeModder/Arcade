/*
** EPITECH PROJECT, 2022
** Arcade: Interface
** File description:
** Creation of Assets and Game objects
*/

/// @file
///
/// Creation of Assets and Game Objects.

#ifndef ARCADE_IASSET_MANAGER_HPP_
#define ARCADE_IASSET_MANAGER_HPP_

#include <memory>
#include <string_view>

#include "IAsset.hpp"
#include "types.hpp"

namespace arcade
{
    class IGameObject;

    /// Loads assets and creates game objects.
    class IAssetManager {
      public:
        virtual ~IAssetManager() = default;

        /// Fetches an asset by name, loading it if necessary.
        ///
        /// The returned IAsset instance is a reference to the real underlying asset, when switching displays, all
        /// existing instances of IAsset will attempt to convert to equivalent assets for the new display mode.
        ///
        /// @param name The name of the requested asset.
        /// @param type Which type of asset to fetch?
        ///
        /// @returns A reference to the loaded asset, or a null reference if the requested asset failed to load.
        virtual std::unique_ptr<IAsset> loadAsset(std::string_view name, IAsset::Type type) = 0;

        /// Creates a text (as in string) object instance.
        ///
        /// @param text The string to display.
        /// @param font The font to use. If @c nullptr, the font isn't used.
        ///
        /// @throws std::logic_error When @c font is not a font asset.
        /// @throws std::logic_error When @c font is @c nulltptr and a TextObject can't be created without a font.
        ///
        /// @returns A boxed IGameObject instance.
        virtual std::unique_ptr<IGameObject> createTextObject(
            std::string_view text, IAsset const *font = nullptr) const = 0;

        /// Creates a textured rectangle object.
        ///
        /// @param size The dimensions (in units) of the rectangle.
        /// @param texture The texture to use. If @c nullptr, the texture isn't used.
        ///
        /// @throws std::logic_error When @c texture is not a texture asset.
        /// @throws std::logic_error When @c texture is @c nullptr and a RectObject can't be created without a texture.
        ///
        /// @returns A boxed IGameObject instance.
        virtual std::unique_ptr<IGameObject> createRectObject(vec2u size, IAsset const *texture = nullptr) const = 0;
    };
} // namespace arcade

#endif /* !ARCADE_IASSET_MANAGER_HPP_ */
