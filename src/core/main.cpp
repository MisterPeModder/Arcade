#include <exception>
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
    try {
        return arcade::arcade(args);
    } catch (std::exception const &error) {
        std::cerr << av[0] << ": fatal error: " << error.what() << std::endl;
    } catch (...) {
        std::cerr << av[0] << ": fatal error" << std::endl;
    }
    return 84;
}
