#ifndef ARCADE_GRAPHICS_SDL2_ASSET_MANAGER
#define ARCADE_GRAPHICS_SDL2_ASSET_MANAGER

#include <memory>
#include <string_view>

#include <arcade/IAsset.hpp>
#include <arcade/IAssetManager.hpp>
#include <arcade/IDisplay.hpp>
#include <arcade/types.hpp>

namespace arcade
{
    class IGameObject;

    class AssetManager : public IAssetManager {
      public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Instantiation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Default constructor.
        AssetManager();

        /// Cannot copy asset managers.
        AssetManager(AssetManager const &) = delete;

        /// Move constructor.
        AssetManager(AssetManager &&) = default;

        /// Move assignment operator.
        AssetManager &operator=(AssetManager &&) = default;

        /// Destructor.
        ~AssetManager() = default;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IAssetManager Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        IDisplay::Type getType() const override final;
        std::unique_ptr<IAsset> loadAsset(std::string_view name, IAsset::Type type) override final;
        std::unique_ptr<IGameObject> createTextObject(std::string_view text, IAsset const *font) const override final;
        std::unique_ptr<IGameObject> createRectObject(vec2u size, IAsset const *texture) const override final;
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_ASSET_MANAGER)
