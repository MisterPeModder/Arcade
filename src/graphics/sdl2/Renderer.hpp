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
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Instantiation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// Default constructor.
        Renderer();

        /// Cannot copy renderer.
        Renderer(Renderer const &) = delete;
        /// Move constructor.
        Renderer(Renderer &&) = default;
        /// Move assignment operator.
        Renderer &operator=(Renderer &&) = default;

        /// Destructor.
        ~Renderer() = default;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IRenderer Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void draw(IGameObject const &object) override final;
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_RENDERER_HPP_)
