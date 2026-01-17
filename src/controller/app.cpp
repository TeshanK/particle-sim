#include "app.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <iostream>

void App::initializeWindow() {

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    m_screenWidth = mode->width;
    m_screenHeight = mode->height;

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(m_screenWidth, m_screenHeight, "Particle Sim",
                              monitor, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, App::framebuffer_size_callback);

    // disable vsync to get the real FPS
    glfwSwapInterval(0);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << '\n';
        return;
    }
    glViewport(0, 0, m_screenWidth, m_screenHeight);
}

void App::Run() {

    initializeWindow();

    if (!window)
        return;

    // Create physics world
    physicsWorld =
        std::make_unique<PhysicsWorld>(m_screenWidth, m_screenHeight);

    // Initialize render system
    if (!physicsWorld->initRenderSystem()) {
        std::cerr << "Failed to initialize render system" << '\n';
        return;
    }

    // Create random particles
    physicsWorld->addRandomParticles(m_particleCount, m_temperature, m_mass);

    // Upload colors of the particles to the GPU
    physicsWorld->uploadColors();

    auto lastTick = getMilliseconds();
    float dt = 0.0;
    float acc = 0.0;
    unsigned int frames = 0;

    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        auto currentTick = getMilliseconds();
        dt = (currentTick - lastTick) / 1000.0f;
        acc += dt;
        if (acc >= 1.0) {
            float fps = frames / acc;
            std::cout << "FPS: " << fps << "\n";

            frames = 0;
            acc = 0.0;
        }
        lastTick = currentTick;
        frames++;

        // Physics update
        physicsWorld->step(dt);

        // Rendering
        physicsWorld->beginFrame();
        physicsWorld->render();
        physicsWorld->endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    physicsWorld->shutdownRenderSystem();
    cleanup();
}

void App::cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void App::processInput(GLFWwindow* win) {
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void App::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);

    // Update physics world dimensions
    App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
    if (app && app->physicsWorld) {
        app->physicsWorld->setDimensions(width, height);
    }
}