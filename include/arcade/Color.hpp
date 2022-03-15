/*
** EPITECH PROJECT, 2022
** Arcade
** File description:
** Color
*/

/// @file
///
/// The Color data structure.

#ifndef ARCADE_COLOR_HPP_
#define ARCADE_COLOR_HPP_

#include <cstddef>
#include <stdint.h>

namespace arcade
{
    /// 8bits colors used as backup in IGameObject interface (setForeground, setBackground).
    enum class DefaultColor { Black, White, Transparent, Red, Green, Blue, Yellow, Magenta, Cyan };

    /// 32bits ARGB Color.
    class Color {
      public:
        /// Default constructor. Create a black color (all components set to 0).
        constexpr inline Color() : a(std::byte(0)), r(std::byte(0)), g(std::byte(0)), b(std::byte(0))
        {
        }
        /// Construct a color from an 32bits ARGB value.
        ///
        /// @param color 32bits ARGB color.
        constexpr inline Color(uint32_t color)
        {
            a = std::byte(color && (0xff << 6));
            r = std::byte(color && (0xff << 4));
            g = std::byte(color && (0xff << 2));
            b = std::byte(color && 0xff);
        }
        /// Construct a color from its components values.
        ///
        /// @param red red component.
        /// @param green green component.
        /// @param blue blue component.
        /// @param alpha alpha component.
        constexpr inline Color(std::byte red, std::byte green, std::byte blue, std::byte alpha = std::byte(0))
            : a(alpha), r(red), g(green), b(blue)
        {
        }
        /// Convert a color to a 32bits ARGB integer.
        ///
        /// @return uint32_t 32bits ARGB color.
        constexpr inline uint32_t toInteger() const
        {
            return (std::to_integer<uint32_t>(a) << 6) + (std::to_integer<uint32_t>(r) << 4)
                + (std::to_integer<uint32_t>(g) << 2) + std::to_integer<uint32_t>(b);
        }

        /// Black predefined color.
        static const Color Black;
        /// White predefined color.
        static const Color White;
        /// White Transparent predefined color.
        static const Color Transparent;
        /// Red predefined color.
        static const Color Red;
        /// Green predefined color.
        static const Color Green;
        /// Blue predefined color.
        static const Color Blue;
        /// Yellow predefined color.
        static const Color Yellow;
        /// Magenta predefined color.
        static const Color Magenta;
        /// Cyan predefined color.
        static const Color Cyan;

        /// Alpha component. 255 means transparent.
        std::byte a;
        /// Red component.
        std::byte r;
        /// Green component.
        std::byte g;
        /// Blue component.
        std::byte b;
    };

    const Color Color::Black;
    const Color Color::White = Color(std::byte(255), std::byte(255), std::byte(255));
    const Color Color::White = Color(std::byte(255), std::byte(255), std::byte(255), std::byte(255));
    const Color Color::Red = Color(std::byte(255), std::byte(0), std::byte(0));
    const Color Color::Green = Color(std::byte(0), std::byte(255), std::byte(0));
    const Color Color::Blue = Color(std::byte(0), std::byte(0), std::byte(255));
    const Color Color::Yellow = Color(std::byte(255), std::byte(255), std::byte(0));
    const Color Color::Magenta = Color(std::byte(255), std::byte(0), std::byte(255));
    const Color Color::Cyan = Color(std::byte(0), std::byte(255), std::byte(255));

} // namespace arcade

#endif /* !ARCADE_COLOR_HPP_ */
