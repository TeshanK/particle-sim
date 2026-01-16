#include "physics_world.h"

bool PhysicsWorld::initRenderSystem() { return renderSystem->init(); }

void PhysicsWorld::shutdownRenderSystem() { renderSystem->shutdown(); }

void PhysicsWorld::addRandomParticles(unsigned int count, float temperature,
                                      float mass) {
    entityManager->makeRandomEntities(count, m_width, m_height, k_pixelsPerMeter,
                                      temperature, mass);
}

void PhysicsWorld::uploadColors() { renderSystem->uploadColors(); }

void PhysicsWorld::setDimensions(unsigned int width, unsigned int height) {
    m_width = width;
    m_height = height;
    m_worldWidthMeters = static_cast<float>(width) / k_pixelsPerMeter;
    m_worldHeightMeters = static_cast<float>(height) / k_pixelsPerMeter;
    collisionSystem->setWorldDimensions(m_worldWidthMeters, m_worldHeightMeters);
}

void PhysicsWorld::step(float dt) {
    motionSystem->update(dt);
    collisionSystem->update();
}

void PhysicsWorld::beginFrame() {
    renderSystem->beginFrame(0.0f, 0.0f, 0.0f, 1.0f, m_width, m_height);
}

void PhysicsWorld::render() { renderSystem->render(); }

void PhysicsWorld::endFrame() { renderSystem->endFrame(); }
