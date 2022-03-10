#include <iostream>

extern "C" void arcadeGameEntryPoint(void)
{
    std::cout << "[game/pacman]: called entry point" << std::endl;
}
