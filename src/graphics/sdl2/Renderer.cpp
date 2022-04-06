#include <arcade/IGameObject.hpp>

#include "Renderer.hpp"
#include "object/Rectangle.hpp"

namespace arcade
{
    Renderer::Renderer() {}

    void Renderer::draw(IGameObject const &object)
    {
        Rectangle const *rect = dynamic_cast<Rectangle const *>(&object);

        if (rect)
            rect->draw();
    }
} // namespace arcade
