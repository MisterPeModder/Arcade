/// @file
/// @internal
///
/// Common aliases.

#ifndef ARCADE_GRAPHICS_COMMON_ALIASES_HPP_
#define ARCADE_GRAPHICS_COMMON_ALIASES_HPP_

#include <memory>

namespace arcade
{
    class IAsset;
    class IGameObject;

    /// IAsset unique pointer.
    using IAssetPtr = std::unique_ptr<IAsset>;
    /// IGameObject unique pointer.
    using IGameObjectPtr = std::unique_ptr<IGameObject>;
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_COMMON_ALIASES_HPP_)
