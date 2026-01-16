#include "motion_system.h"

void MotionSystem::update(float dt) {
    auto &positions = m_entityManager->transform.Positions;
    auto &velocities = m_entityManager->physics.Velocities;
    auto &forces = m_entityManager->physics.Forces;
    auto &masses = m_entityManager->physics.Masses;

    for (unsigned int i = 0; i < m_entityManager->entityCount; i++) {
        // Update velocity based on force (F = ma => a = F/m)
        velocities[i] += (forces[i] / masses[i]) * dt;

        // Update position based on velocity
        positions[i] += velocities[i] * dt;

        // Reset force for next frame
        forces[i] = glm::vec2(0.0f, 0.0f);
    }
}