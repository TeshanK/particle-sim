#include "render_system.h"

bool RenderSystem::init() {
    // Create shader
    m_shader =
        std::make_unique<Shader>("../src/shaders/circle_instance.vert.glsl",
                                 "../src/shaders/circle_instance.frag.glsl");

    // Quad geometry centered at origin, side lengths 1 (maps to UV in shader)
    float vertices[] = {0.5f,  0.5f,  0.0f, 0.5f,  -0.5f, 0.0f,
                        -0.5f, -0.5f, 0.0f, -0.5f, 0.5f,  0.0f};
    unsigned int indices[] = {0, 1, 3, 1, 2, 3};

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_positionSSBO);
    glGenBuffers(1, &m_colorSSBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // Enable blending for smooth edges
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void RenderSystem::shutdown() {
    if (m_EBO)
        glDeleteBuffers(1, &m_EBO);
    if (m_VBO)
        glDeleteBuffers(1, &m_VBO);
    if (m_VAO)
        glDeleteVertexArrays(1, &m_VAO);
    if (m_colorSSBO)
        glDeleteBuffers(1, &m_colorSSBO);
    if (m_positionSSBO)
        glDeleteBuffers(1, &m_positionSSBO);
    m_positionSSBO = m_colorSSBO = m_EBO = m_VBO = m_VAO = 0;
}

void RenderSystem::beginFrame(float r, float g, float b, float a, int width,
                              int height) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);

    // Build an orthographic projection where 1 unit == 1 meter in physics space
    float worldWidthMeters = static_cast<float>(width) / m_pixelsPerMeter;
    float worldHeightMeters = static_cast<float>(height) / m_pixelsPerMeter;
    m_projection = glm::ortho(0.0f, worldWidthMeters, 0.0f, worldHeightMeters);

    m_shader->use();
    m_shader->setMat4("projection", m_projection);
    m_shader->setFloat("radius", m_radius);
}

void RenderSystem::render() {
    // Upload positions to SSBO
    const auto &positions = m_entityManager->transform.Positions;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_positionSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, positions.size() * sizeof(glm::vec2),
                 positions.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_positionSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Draw instanced
    glBindVertexArray(m_VAO);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0,
                            m_entityManager->entityCount);
}

void RenderSystem::uploadColors() {
    // Convert vec3 colors to vec4 for shader
    const auto &colors = m_entityManager->properties.Colors;
    std::vector<glm::vec4> colorData(colors.size());
    for (size_t i = 0; i < colors.size(); ++i) {
        colorData[i] = glm::vec4(colors[i], 1.0f);
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_colorSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, colorData.size() * sizeof(glm::vec4),
                 colorData.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_colorSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void RenderSystem::endFrame() {
    // no-op; Application swaps buffers and polls events
}