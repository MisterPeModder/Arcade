#include <arcade/Color.hpp>
#include <arcade/IGameObject.hpp>
#include <arcade/types.hpp>

#include "../asset/Texture.hpp"
#include "Rectangle.hpp"

namespace arcade
{
    Rectangle::Rectangle() : _dims({0, 0, 0, 0}), _texture(nullptr)
    {
    }

    Rectangle::Rectangle(vec2u size, Texture const *texture)
        : _dims({0, 0, static_cast<int>(size.x), static_cast<int>(size.y)}), _texture(texture)
    {
    }

    Rectangle::Rectangle(Rectangle &&other) : _dims(other._dims), _texture(other._texture)
    {
        other._dims = {0, 0, 0, 0};
        other._texture = nullptr;
    }

    Rectangle &Rectangle::operator=(Rectangle &&other)
    {
        this->_dims = other._dims;
        this->_texture = other._texture;
        other._dims = {0, 0, 0, 0};
        other._texture = nullptr;
        return *this;
    }

    void Rectangle::draw() const
    {
        if (this->_texture)
            this->_texture->render(this->_dims);
    }

    IGameObject::Type Rectangle::getType() const
    {
        return IGameObject::Type::Rect;
    }

    vec2u Rectangle::getSize() const
    {
        return {static_cast<unsigned int>(this->_dims.w), static_cast<unsigned int>(this->_dims.h)};
    }

    vec2i Rectangle::getPosition() const
    {
        return {this->_dims.x, this->_dims.y};
    }

    void Rectangle::setPosition(vec2i pos)
    {
        this->_dims.x = pos.x;
        this->_dims.y = pos.y;
    }

    void Rectangle::setForeground(Color color, DefaultColor)
    {
        (void)color;
        // if (this->_texture)
        //     this->_texture->tint(color);
    }

    void Rectangle::setBackground(Color, DefaultColor)
    {
    }
} // namespace arcade
