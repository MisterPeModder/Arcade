/// @file
/// @internal
///
/// Internal arcade core header file.

#ifndef ARCADE_CORE_ARCADE_HPP_
#define ARCADE_CORE_ARCADE_HPP_

#include <span>
#include <string>

namespace arcade
{
    /// Arcade's main function.
    ///
    /// @param args The program's arguments.
    ///
    /// @return The program's exit value.
    ///
    /// @throws anything... no, seriously!
    int arcade(std::span<std::string> args);
} // namespace arcade

#endif // !defined(ARCADE_CORE_ARCADE_HPP_)
