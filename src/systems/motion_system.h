#ifndef MOTION_SYSTEM_H
#define MOTION_SYSTEM_H

#include "../controller/entity_manager.h"
#include <glm/glm.hpp>
#include <vector>

class MotionSystem {
  public:
    MotionSystem(EntityManager* entityManager)
        : m_entityManager{entityManager},
          positions{m_entityManager->transform.Positions},
          velocities{m_entityManager->physics.Velocities},
          forces{m_entityManager->physics.Forces},
          masses{m_entityManager->physics.Masses} {}

    void update(float dt);

  private:
    // Keep m_entityManger the first since members are initialized in the order
    // they are declared in the class
    EntityManager* m_entityManager;

    std::vector<glm::vec2>& positions;
    std::vector<glm::vec2>& velocities;
    std::vector<glm::vec2>& forces;
    std::vector<float>& masses;
};

#endif // MOTION_SYSTEM_H