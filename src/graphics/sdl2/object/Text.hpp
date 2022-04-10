/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_SDL2_OBJECT_TEXT_HPP_
#define ARCADE_GRAPHICS_SDL2_OBJECT_TEXT_HPP_

#include <arcade/IMutableText.hpp>

#include "Rectangle.hpp"

namespace arcade
{
    class Text : public Rectangle, public IMutableText {
      public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Instantiation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Text();

        /// Cannot copy text.
        Text(Text const &) = delete;

        /// Move constructor.
        Text(Text &&) = default;

        /// Move assingment operator.
        Text &operator=(Text &&) = default;

        /// Destructor.
        ~Text() = default;

        static std::unique_ptr<Text> create(SDL_Renderer *renderer, Font const *font, std::string_view text);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IGameObject Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Type getType() const override final;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // IMutableText Implementation
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void setText(std::string_view text) override final;

      private:
        Font const *_font;

        explicit Text(SDL_Renderer *renderer, Font const *font, SDL_Texture *sprite, vec2u size);
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_SDL2_OBJECT_TEXT_HPP_)
