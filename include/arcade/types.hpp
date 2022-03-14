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
    class IGame;
    class IDisplay;

    /// A 2D vector.
    template <typename T>
    struct vec2
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

    ///
    /// @brief Type of the entry point of the library to get an instance of IGame.
    ///        The function used as EntryPoint must be named as the GameEntryPointName below.
    ///
    ///
    using GameEntryPoint = IGame *(*)();
    ///
    /// @brief Expected name of the Game entry point.
    ///
    ///
    constexpr std::string_view GameEntryPointName = "arcade_GameEntryPoint";
    ///
    /// @brief Type of the entry point of the library to get an instance of IGame.
    ///        The function used as EntryPoint must be named as the DisplayEntryPointName below.
    ///
    ///
    using DisplayEntryPoint = IDisplay *(*)();
    ///
    /// @brief Expected name of the Display entry point.
    ///
    ///
    constexpr std::string_view DisplayEntryPointName = "arcade_DisplayEntryPoint";
} // namespace arcade

#endif // !defined(ARCADE_TYPES_HPP_)
