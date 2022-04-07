/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SDL2_RENDERER_HPP_
#define ARCADE_GRAPHICS_SDL2_RENDERER_HPP_

#include <SFML/Graphics/RenderStates.hpp>

#include <arcade/IRenderer.hpp>

namespace sf
{
    class RenderTarget;
} // namespace sf

namespace arcade
{
    class IGameObject;

    class Renderer : public IRenderer {
      public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Instantiation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Renderer(sf::RenderTarget &target, sf::RenderStates const &renderStates);

        /// Cannot copy renderers.
        Renderer(Renderer const &) = delete;

        /// Move constructor.
        Renderer(Renderer &&) = default;

        /// Assignment move operator.
        Renderer &operator=(Renderer &&) = delete;

        /// Destructor.
        ~Renderer() = default;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IRenderer implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void draw(IGameObject const &object) override final;

      private:
        sf::RenderTarget &_target;
        sf::RenderStates const &_renderStates;
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_RENDERER_HPP_)
