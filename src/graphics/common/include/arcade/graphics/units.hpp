/// @file
/// @internal
///
/// Common unit conversions.

#ifndef ARCADE_GRAPHICS_COMMON_UNITS_HPP_
#define ARCADE_GRAPHICS_COMMON_UNITS_HPP_

#include <arcade/types.hpp>

namespace arcade
{
    constexpr int PIXELS_PER_UNIT = 25;

    /// Converts a vector of pixels to a vector of units.
    template <typename T> constexpr T toUnits(T pixels) { return pixels / PIXELS_PER_UNIT; }

    /// Converts a vector of units to a vector of pixels.
    template <typename T> constexpr T toPixels(T units) { return units * PIXELS_PER_UNIT; }

    /// The default window size, in units.
    static constexpr vec2u WINDOW_SIZE_UNITS = {59, 35};
    /// The default window size, in pixels.
    static constexpr vec2u WINDOW_SIZE_PIXELS = toPixels(WINDOW_SIZE_UNITS);

} // namespace arcade

#endif // !defined(ARCADE_GRAPHICS_COMMON_UNITS_HPP_)
