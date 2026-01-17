#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include "../systems/collision_system.h"
#include "../systems/motion_system.h"
#include "../systems/render_system.h"
#include "entity_manager.h"
#include <memory>

constexpr float k_pixelsPerMeter = 100.0f;
constexpr float k_defaultRadius = 0.015f;

class PhysicsWorld {
  public:
    PhysicsWorld(int width, int height)
        : m_width{width}, m_height{height},
          m_worldWidthMeters{static_cast<float>(width) / k_pixelsPerMeter},
          m_worldHeightMeters{static_cast<float>(height) / k_pixelsPerMeter},
          entityManager{std::make_unique<EntityManager>()},
          motionSystem{std::make_unique<MotionSystem>(entityManager.get())},
          collisionSystem{std::make_unique<CollisionSystem>(
              entityManager.get(), m_worldWidthMeters, m_worldHeightMeters)},
          renderSystem{std::make_unique<RenderSystem>(
              entityManager.get(), k_defaultRadius, k_pixelsPerMeter)} {}

    bool initRenderSystem();
    void shutdownRenderSystem();

    void addRandomParticles(unsigned int count, float temperature, float mass);
    void uploadColors();

    void setDimensions(int width, int height);

    void step(float dt);
    void beginFrame();
    void render();
    void endFrame();

    [[nodiscard]] float getWorldWidthMeters() const {
        return m_worldWidthMeters;
    }
    [[nodiscard]] float getWorldHeightMeters() const {
        return m_worldHeightMeters;
    }

  private:
    int m_width{800};
    int m_height{600};
    float m_worldWidthMeters{0.0f};
    float m_worldHeightMeters{0.0f};

    std::unique_ptr<EntityManager> entityManager = nullptr;
    std::unique_ptr<MotionSystem> motionSystem = nullptr;
    std::unique_ptr<CollisionSystem> collisionSystem = nullptr;
    std::unique_ptr<RenderSystem> renderSystem = nullptr;
};

#endif // PHYSICS_WORLD_H