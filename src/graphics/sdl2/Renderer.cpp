#include <arcade/IGameObject.hpp>

#include "Renderer.hpp"
#include "object/Rectangle.hpp"

namespace arcade
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Instantiation
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Renderer::Renderer() {}

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // IRenderer Implementation
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Renderer::draw(IGameObject const &object)
    {
        Rectangle const *rect = dynamic_cast<Rectangle const *>(&object);

        if (rect)
            rect->draw();
    }
} // namespace arcade
