/// @file
///
/// Internal arcade core header file.

#ifndef ARCADE_CORE_HPP_
#define ARCADE_CORE_HPP_

#include <span>
#include <string_view>

/// Arcade's main function.
///
/// @param args The program's arguments.
///
/// @return The program's exit value.
int arcade(std::span<std::string_view> args);

#endif // !defined(ARCADE_CORE_HPP_)
