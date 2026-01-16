#ifndef MOTION_SYSTEM_H
#define MOTION_SYSTEM_H

#include "../controller/entity_manager.h"
#include <glm/glm.hpp>

class MotionSystem {
  public:
    MotionSystem(EntityManager *entityManager)
        : m_entityManager{entityManager} {}

    void update(float dt);

  private:
    EntityManager *m_entityManager;
};

#endif // MOTION_SYSTEM_H