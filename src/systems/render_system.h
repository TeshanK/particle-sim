#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../controller/entity_manager.h"
#include "../shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class RenderSystem {
  public:
    RenderSystem(EntityManager* entityManager, float radius,
                 float pixelsPerMeter = 100.0f)
        : m_entityManager{entityManager}, m_radius{radius},
          m_pixelsPerMeter{pixelsPerMeter} {}

    bool init();
    void shutdown();

    void beginFrame(float r, float g, float b, float a, int width, int height);
    void render();
    void endFrame();

    void uploadColors();
    [[nodiscard]] float getRadius() const { return m_radius; }

  private:
    EntityManager* m_entityManager;
    float m_radius{0.015f};
    float m_pixelsPerMeter{100.0f};

    GLuint m_VAO{0}, m_VBO{0}, m_EBO{0};
    GLuint m_positionSSBO{0}, m_colorSSBO{0};
    glm::mat4 m_projection{1.0f};
    std::unique_ptr<Shader> m_shader;

    // Cached uniform locations
    GLint projectionLoc;
    GLint radiusLoc;
};

#endif // RENDER_SYSTEM_H