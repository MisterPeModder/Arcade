#include <iostream>

extern "C" void arcadeGraphicsEntryPoint(void)
{
    std::cout << "[graphics/sfml]: called entry point" << std::endl;
}
