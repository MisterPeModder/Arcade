/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SDL2_RENDERER_HPP_
#define ARCADE_GRAPHICS_SDL2_RENDERER_HPP_

#include <arcade/IRenderer.hpp>

namespace arcade
{
    class IGameObject;

    class Renderer : public IRenderer {
      public:
        Renderer();

        Renderer(Renderer const &) = delete;
        Renderer(Renderer &&) = default;
        Renderer &operator=(Renderer &&) = default;

        ~Renderer() = default;

        void draw(IGameObject const &object) override final;
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_RENDERER_HPP_)
