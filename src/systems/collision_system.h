#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../controller/entity_manager.h"
#include <glm/glm.hpp>

class CollisionSystem {
  public:
    CollisionSystem(EntityManager *entityManager, float worldWidth,
                    float worldHeight)
        : m_entityManager{entityManager}, m_worldWidth{worldWidth},
          m_worldHeight{worldHeight} {}

    void setWorldDimensions(float width, float height) {
        m_worldWidth = width;
        m_worldHeight = height;
    }

    void update(float damping = 1.0f);

  private:
    EntityManager *m_entityManager;
    float m_worldWidth{0.0f};
    float m_worldHeight{0.0f};

    void handleWallCollisions(float damping);
    void handleObjectCollisions(float damping);
};

#endif // COLLISION_SYSTEM_H