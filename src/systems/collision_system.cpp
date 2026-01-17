#include "collision_system.h"
#include <glm/geometric.hpp>

void CollisionSystem::update(float damping) {
    handleWallCollisions(damping);
    handleObjectCollisions(damping);
}

void CollisionSystem::handleWallCollisions(float damping) {
    for (unsigned int i = 0; i < m_entityManager->entityCount; ++i) {
        float radius = radii[i];

        // Left wall
        if (positions[i].x - radius < 0) {
            positions[i].x = radius;
            velocities[i].x = -velocities[i].x * damping;
        }

        // Right wall
        if (positions[i].x + radius > m_worldWidth) {
            positions[i].x = m_worldWidth - radius;
            velocities[i].x = -velocities[i].x * damping;
        }

        // Top wall
        if (positions[i].y + radius > m_worldHeight) {
            positions[i].y = m_worldHeight - radius;
            velocities[i].y = -velocities[i].y * damping;
        }

        // Bottom wall
        if (positions[i].y - radius < 0) {
            positions[i].y = radius;
            velocities[i].y = -velocities[i].y * damping;
        }
    }
}

void CollisionSystem::handleObjectCollisions(float damping) {
    const int n = m_entityManager->entityCount;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            glm::vec2 x1 = positions[i];
            glm::vec2 x2 = positions[j];

            float r1 = radii[i];
            float r2 = radii[j];

            float dist = glm::length(x2 - x1);
            if (dist <= r1 + r2 && dist > 0.0f) {
                glm::vec2 v1 = velocities[i];
                glm::vec2 v2 = velocities[j];

                float m1 = masses[i];
                float m2 = masses[j];

                // Separate overlapping particles
                glm::vec2 normal = glm::normalize(x2 - x1);
                float overlap = (r1 + r2 - dist) / 2.0f;

                positions[i] -= overlap * normal;
                positions[j] += overlap * normal;

                // Update positions for velocity calculation
                x1 = positions[i];
                x2 = positions[j];

                // Elastic collision response
                glm::vec2 dx = x1 - x2;
                float dotProduct = glm::dot(x1 - x2, x1 - x2);

                if (dotProduct > 0.0f) {
                    velocities[i] =
                        v1 - (2 * m2 / (m1 + m2)) *
                                 (glm::dot(v1 - v2, x1 - x2) / dotProduct) *
                                 (x1 - x2);
                    velocities[j] =
                        v2 - (2 * m1 / (m1 + m2)) *
                                 (glm::dot(v2 - v1, x2 - x1) / dotProduct) *
                                 (x2 - x1);

                    velocities[i] *= damping;
                    velocities[j] *= damping;
                }
            }
        }
    }
}