#include <arcade/IGameObject.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Renderer.hpp"

namespace arcade
{
    Renderer::Renderer(sf::RenderTarget &target, sf::RenderStates const &renderStates)
        : _target(target), _renderStates(renderStates)
    {
    }

    void Renderer::draw(IGameObject const &object)
    {
        sf::Drawable const *drawable = dynamic_cast<sf::Drawable const *>(&object);

        if (drawable)
            this->_target.draw(*drawable, this->_renderStates);
    }
} // namespace arcade
