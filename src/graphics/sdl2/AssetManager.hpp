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
        AssetManager(SDL_Renderer *renderer);

        AssetManager(AssetManager const &) = delete;
        AssetManager(AssetManager &&);
        AssetManager &operator=(AssetManager &&);

        ~AssetManager() = default;

        IDisplay::Type getType() const override final;

        std::unique_ptr<IAsset> loadAsset(std::string_view name, IAsset::Type type) override final;

        std::unique_ptr<IGameObject> createTextObject(std::string_view text, IAsset const *font) const override final;

        std::unique_ptr<IGameObject> createRectObject(vec2u size, IAsset const *texture) const override final;

      private:
        SDL_Renderer *_renderer;
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_ASSET_MANAGER)
