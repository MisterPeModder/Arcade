/*
** EPITECH PROJECT, 2022
** Arcade: Interface
** File description:
** Misc types
*/

/// @file
///
/// Misc types.

#ifndef ARCADE_TYPES_HPP_
#define ARCADE_TYPES_HPP_

namespace arcade
{
    /// A 2D vector.
    template <typename T> struct vec2
    {
        /// x coordinate.
        T x;
        /// y coordinate.
        T y;
    };

    /// A 2D vector of unsigned ints.
    using vec2u = vec2<unsigned int>;

    /// A 2D vector of ints.
    using vec2i = vec2<int>;
} // namespace arcade

#endif // !defined(ARCADE_TYPES_HPP_)
