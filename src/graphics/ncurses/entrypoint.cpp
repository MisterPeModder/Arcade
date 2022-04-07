#include <iostream>

extern "C" void arcadeGraphicsEntryPoint(void) { std::cout << "[graphics/ncurses]: called entry point" << std::endl; }
