#ifndef APP_H
#define APP_H

#include "physics_world.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// clang-format on

#include <chrono>
#include <memory>

class App {
  public:
    App(unsigned int particleCount, float temperature = 0.125f,
        float mass = 1.0f, int screenWidth = 800,
        int screenHeight = 600)
        : m_particleCount{particleCount}, m_temperature{temperature},
          m_mass{mass}, m_screenWidth{screenWidth},
          m_screenHeight{screenHeight} {}

    void Run();

  private:
    unsigned int m_particleCount;
    float m_temperature;
    float m_mass;
    int m_screenWidth, m_screenHeight;

    GLFWwindow* window = nullptr;
    std::unique_ptr<PhysicsWorld> physicsWorld = nullptr;

    void initializeWindow();
    void cleanup();
    void processInput(GLFWwindow* win);
    static void framebuffer_size_callback(GLFWwindow* window, int width,
                                          int height);
};

inline auto getMilliseconds() -> long {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return milliseconds;
}

#endif // APP_H