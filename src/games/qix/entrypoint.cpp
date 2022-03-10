#include <iostream>

extern "C" void arcadeGameEntryPoint(void)
{
    std::cout << "[game/qix]: called entry point" << std::endl;
}
