#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>
#include <vector>

struct Physics {
    std::vector<glm::vec2> Velocities;
    std::vector<glm::vec2> Forces;
    std::vector<float> Masses;
    std::vector<float> Radii;
};

#endif // PHYSICS_H