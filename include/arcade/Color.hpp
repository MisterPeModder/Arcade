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
#include <cstdint>

namespace arcade
{
    /// Default colors used as backup in IGameObject interface (setForeground, setBackground).
    enum class DefaultColor {
        Black,
        White,
        Transparent,
        Red,
        Green,
        Blue,
        Yellow,
        Magenta,
        Cyan,
    };

    /// 32-bit ARGB Color.
    class Color {
      public:
        /// Default constructor. Create a black color (all components set to 0).
        constexpr inline Color() : a(std::byte(0)), r(std::byte(0)), g(std::byte(0)), b(std::byte(0))
        {
        }

        /// Construct a color from an 32-bit ARGB value.
        ///
        /// @param color 32-bit ARGB color.
        constexpr inline Color(uint32_t color)
            : a(std::byte((color & (0xff << 24)) >> 24)), r(std::byte((color & (0xff << 16)) >> 16)),
              g(std::byte((color & (0xff << 8)) >> 8)), b(std::byte(color & 0xff))
        {
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

        /// Convert a color to a 32-bit ARGB integer.
        ///
        /// @return uint32_t 32-bit ARGB color.
        constexpr inline uint32_t toInteger() const
        {
            return (std::to_integer<uint32_t>(a) << 24) | (std::to_integer<uint32_t>(r) << 16)
                | (std::to_integer<uint32_t>(g) << 8) | std::to_integer<uint32_t>(b);
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

    constexpr Color Color::Black = Color(0x00000000);
    constexpr Color Color::White = Color(0x00ffffff);
    constexpr Color Color::Transparent = Color(0xffffff);
    constexpr Color Color::Red = Color(0x00ff0000);
    constexpr Color Color::Green = Color(0x0000ff00);
    constexpr Color Color::Blue = Color(0x000000ff);
    constexpr Color Color::Yellow = Color(0x00ffff00);
    constexpr Color Color::Magenta = Color(0x00ff00ff);
    constexpr Color Color::Cyan = Color(0x0000ffff);

} // namespace arcade

#endif /* !ARCADE_COLOR_HPP_ */
