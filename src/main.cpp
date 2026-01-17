#include "controller/app.h"

#include <iostream>
#include <memory>

int main() {
    std::cout << "Starting Particle Simulation\n";

#ifndef NDEBUG
    const unsigned int particleCount = 10;
#else
    const unsigned int particleCount = 6400;
#endif

    std::unique_ptr<App> app = std::make_unique<App>(particleCount);
    app->Run();

    return 0;
}