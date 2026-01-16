#include "controller/app.h"

#include <iostream>
#include <memory>

int main() {
    std::cout << "Starting Particle Simulation\n";

    std::unique_ptr<App> app = std::make_unique<App>(6400);
    app->Run();

    return 0;
}