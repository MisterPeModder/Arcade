#ifndef ARCADE_GRAPHICS_SDL2_ASSET_MANAGER
#define ARCADE_GRAPHICS_SDL2_ASSET_MANAGER

#include <memory>
#include <string_view>

#include <SDL_render.h>

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

        AssetManager(SDL_Renderer *renderer);

        /// Cannot copy asset manager.
        AssetManager(AssetManager const &) = delete;
        /// Move constructor.
        AssetManager(AssetManager &&);
        /// Move assignment operator.
        AssetManager &operator=(AssetManager &&);

        /// Destructor.
        ~AssetManager() = default;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IAssetManager Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        IDisplay::Type getType() const override final;
        std::unique_ptr<IAsset> loadAsset(std::string_view name, IAsset::Type type) override final;
        std::unique_ptr<IGameObject> createTextObject(std::string_view text, IAsset const *font) const override final;
        std::unique_ptr<IGameObject> createRectObject(vec2u size, IAsset const *texture) const override final;

      private:
        SDL_Renderer *_renderer;
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_ASSET_MANAGER)
