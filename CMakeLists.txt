cmake_minimum_required(VERSION 3.16...3.41)
project(particle-sim LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -DNDEBUG -march=native -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")

# =============================================================================
# External Dependencies
# =============================================================================
include(FetchContent)

# ---------- GLFW ----------
message(STATUS "Fetching GLFW...")
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# ---------- GLM ----------
message(STATUS "Fetching GLM...")
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glm)

# ---------- GLAD ----------
message(STATUS "Using local GLAD...")
add_subdirectory(external/glad glad)

# =============================================================================
# Main Executable
# =============================================================================
add_executable(particle-sim
    src/main.cpp
    src/controller/app.cpp
    src/controller/entity_manager.cpp
    src/controller/physics_world.cpp
    src/systems/motion_system.cpp
    src/systems/collision_system.cpp
    src/systems/render_system.cpp
)

target_include_directories(particle-sim
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(particle-sim
    PRIVATE
        glfw
        glad
        glm::glm
)

# Platform-specific libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(particle-sim PRIVATE dl pthread)
endif()