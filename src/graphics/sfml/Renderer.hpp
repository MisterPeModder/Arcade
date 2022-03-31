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
        Renderer(sf::RenderTarget &target, sf::RenderStates const &renderStates);

        Renderer(Renderer const &) = delete;
        Renderer(Renderer &&) = default;
        Renderer &operator=(Renderer &&) = delete;

        ~Renderer() = default;

        void draw(IGameObject const &object) override final;

      private:
        sf::RenderTarget &_target;
        sf::RenderStates const &_renderStates;
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_RENDERER_HPP_)
