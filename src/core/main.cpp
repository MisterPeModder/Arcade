#include <iostream>
#include <string>
#include <vector>

#include "core.hpp"

int main(int ac, char *av[])
{
    std::vector<std::string> args;

    args.reserve(ac);
    for (int i = 0; i < ac; ++i)
        args.emplace_back(av[i]);
    return arcade::arcade(args);
}
