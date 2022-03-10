#include <iostream>

extern "C" void arcadeGameEntryPoint(void)
{
    std::cout << "[game/nibbler]: called entry point" << std::endl;
}
