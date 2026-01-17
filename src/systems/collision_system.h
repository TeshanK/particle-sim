#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../controller/entity_manager.h"
#include <glm/glm.hpp>
#include <vector>

class CollisionSystem {
  public:
    CollisionSystem(EntityManager* entityManager, float worldWidth,
                    float worldHeight)
        : m_entityManager{entityManager},
          positions{m_entityManager->transform.Positions},
          velocities{m_entityManager->physics.Velocities},
          masses{m_entityManager->physics.Masses},
          radii{m_entityManager->physics.Radii}, m_worldWidth{worldWidth},
          m_worldHeight{worldHeight} {}

    void setWorldDimensions(float width, float height) {
        m_worldWidth = width;
        m_worldHeight = height;
    }

    void update(float damping = 1.0f);

  private:
    EntityManager* m_entityManager;

    std::vector<glm::vec2>& positions;
    std::vector<glm::vec2>& velocities;
    std::vector<float>& masses;
    std::vector<float>& radii;

    float m_worldWidth{0.0f};
    float m_worldHeight{0.0f};

    void handleWallCollisions(float damping);
    void handleObjectCollisions(float damping);
};

#endif // COLLISION_SYSTEM_H