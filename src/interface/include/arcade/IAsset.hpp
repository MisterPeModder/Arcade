/*
** EPITECH PROJECT, 2022
** Arcade: Interface
** File description:
** The Asset interface
*/

/// @file
///
/// The asset interface.

#ifndef ARCADE_IASSET_HPP_
#define ARCADE_IASSET_HPP_

namespace arcade
{
    /// Generic asset.
    ///
    /// An asset can be any a font, an image, a texture, and more.
    class IAsset {
      public:
        /// The type of an asset.
        enum class Type {
            /// A font.
            Font,
            /// A graphical sprite made of pixels.
            Texture,
            /// A set of characters that acts as a sprite (aka: ASCII-art).
            CharSet,
        };

        virtual ~IAsset() = default;

        /// @returns The type of this asset.
        virtual Type getType() const = 0;
    };
} // namespace arcade

#endif // !defined(ARCADE_IASSET_HPP_)
