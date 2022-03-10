#include <iostream>

#include "arcade/core.hpp"
#include "arcade/interface.hpp"

int arcade(std::span<std::string_view> args)
{
    std::cout << "starting arcade...\n";
    std::cout << "args: \n";

    for (auto arg : args)
        std::cout << "- " << arg << '\n';
    std::cout.flush();
    interface_dummy();
    return 0;
}
