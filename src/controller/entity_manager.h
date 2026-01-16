#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "../components/physics.h"
#include "../components/properties.h"
#include "../components/transform.h"

#include <glm/glm.hpp>

class EntityManager {
  public:
    unsigned int entityCount{0};

    Transform transform;
    Physics physics;
    Properties properties;

    EntityManager() = default;

    void makeEntity(glm::vec2 position, glm::vec2 velocity, glm::vec3 color,
                    float mass, float radius);
    void makeRandomEntities(unsigned int count, unsigned int screenWidth,
                            unsigned int screenHeight, float pixelsPerMeter,
                            float temperature, float mass);
};

#endif // ENTITY_MANAGER_H