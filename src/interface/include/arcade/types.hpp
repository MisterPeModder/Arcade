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
    template <typename T> struct vec2 {
        /// x coordinate.
        T x;
        /// y coordinate.
        T y;

        /// Direct vector casting support.
        template <typename U> explicit constexpr operator vec2<U>() const
        {
            return vec2<U>{static_cast<U>(x), static_cast<U>(y)};
        }

        constexpr vec2<T> operator+() const
        {
            return *this;
        }

        constexpr vec2<T> operator-() const
        {
            return vec2<T>{-x, -y};
        }

        constexpr vec2<T> operator+(T const &v) const
        {
            return vec2<T>{x + v, y + v};
        }

        constexpr vec2<T> operator+(vec2<T> const &v) const
        {
            return vec2<T>{x + v.x, y + v.y};
        }

        constexpr vec2<T> operator-(T const &v) const
        {
            return vec2<T>{x - v, y - v};
        }

        constexpr vec2<T> operator-(vec2<T> const &v) const
        {
            return vec2<T>{x - v.x, y - v.y};
        }

        constexpr vec2<T> operator*(T const &v) const
        {
            return vec2<T>{x * v, y * v};
        }

        /// Dot product.
        constexpr T operator*(vec2<T> const &v) const
        {
            return x * v.x + y * v.y;
        }

        constexpr vec2<T> operator/(T const &v) const
        {
            return vec2<T>{x / v, y / v};
        }

        constexpr vec2<T> &operator+=(T const &v)
        {
            x += v;
            y += v;
            return *this;
        }

        constexpr vec2<T> &operator+=(vec2<T> const &v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }

        constexpr vec2<T> &operator-=(T const &v)
        {
            x -= v;
            y -= v;
            return *this;
        }

        constexpr vec2<T> &operator-=(vec2<T> const &v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        constexpr vec2<T> &operator*=(T const &v)
        {
            x *= v;
            y *= v;
            return *this;
        }

        constexpr vec2<T> &operator/=(T const &v)
        {
            x /= v;
            y /= v;
            return *this;
        }
    };

    /// A 2D vector of unsigned ints.
    using vec2u = vec2<unsigned int>;

    /// A 2D vector of ints.
    using vec2i = vec2<int>;
} // namespace arcade

#endif // !defined(ARCADE_TYPES_HPP_)
