/// @file
/// @internal

#ifndef ARCADE_GRAPHICS_COMMON_IMUTABLE_TEXT_HPP_
#define ARCADE_GRAPHICS_COMMON_IMUTABLE_TEXT_HPP_

#include <string_view>

namespace arcade
{
    /// Extension for IGameObject. Allows the text of object to be modified.
    class IMutableText {
      public:
        virtual ~IMutableText() = default;

        virtual void setText(std::string_view text) = 0;
    };
} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_COMMON_IMUTABLE_TEXT_HPP_)
