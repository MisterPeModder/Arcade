#include <iostream>
#include <vector>
#include <string_view>

#include "arcade/core.hpp"

int main(int ac, char *av[])
{
    std::vector<std::string_view> args;

    args.reserve(ac);
    for (int i = 0; i < ac; ++i)
        args.emplace_back(av[i]);
    return arcade(args);
}
