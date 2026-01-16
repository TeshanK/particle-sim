#include "entity_manager.h"
#include <cmath>
#include <random>

void EntityManager::makeEntity(glm::vec2 position, glm::vec2 velocity,
                               glm::vec3 color, float mass, float radius) {
    entityCount++;

    transform.Positions.push_back(position);
    physics.Velocities.push_back(velocity);
    physics.Forces.emplace_back(0.0f, 0.0f);
    physics.Masses.push_back(mass);
    physics.Radii.push_back(radius);
    properties.Colors.push_back(color);
}

static float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

static float randomGaussian(float mean, float stddev) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<float> dis(mean, stddev);
    return dis(gen);
}

void EntityManager::makeRandomEntities(unsigned int count,
                                       unsigned int screenWidth,
                                       unsigned int screenHeight,
                                       float pixelsPerMeter, float temperature,
                                       float mass) {
    const float XPOS_LOW = 0;
    const float XPOS_HIGH = std::floor(screenWidth / pixelsPerMeter);

    const float YPOS_LOW = 0;
    const float YPOS_HIGH = std::floor(screenHeight / pixelsPerMeter);

    const float RADIUS = 0.015f;

    // velocity standard deviation from temperature and mass
    const float velocityStddev = std::sqrt(temperature / mass);

    for (unsigned int i = 0; i < count; ++i) {
        glm::vec2 position =
            glm::vec2(randomFloat(XPOS_LOW + RADIUS, XPOS_HIGH - RADIUS),
                      randomFloat(YPOS_LOW + RADIUS, YPOS_HIGH - RADIUS));

        glm::vec2 velocity = glm::vec2(randomGaussian(0.0f, velocityStddev),
                                       randomGaussian(0.0f, velocityStddev));

        glm::vec3 color =
            glm::vec3(randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f),
                      randomFloat(0.0f, 1.0f));

        makeEntity(position, velocity, color, mass, RADIUS);
    }
}