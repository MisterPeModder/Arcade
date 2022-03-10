#include <iostream>

extern "C" void arcadeGraphicsEntryPoint(void)
{
    std::cout << "[graphics/sdl2]: called entry point" << std::endl;
}
