cmake_minimum_required(VERSION 3.16...3.41)
project(particle-sim LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -march=native -g -fno-omit-frame-pointer -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

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

# Apply strict warnings only to particle-sim
target_compile_options(particle-sim PRIVATE -Wall -Wextra -Wconversion -Wshadow)

# Enable UBSan and ASan only for debug builds
target_compile_options(particle-sim PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=undefined,address -fno-omit-frame-pointer -g>
)
target_link_options(particle-sim PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=undefined,address>
)

# Enable clang-tidy only for particle-sim
set_target_properties(particle-sim PROPERTIES 
    CXX_CLANG_TIDY "clang-tidy-21;-checks=-*,bugprone-*,cert-*,clang-analyzer-*,concurrency-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-readability-identifier-length,-readability-uppercase-literal-suffix,-readability-braces-around-statements,-readability-implicit-bool-conversion,-readability-magic-numbers,-readability-convert-member-functions-to-static,-bugprone-easily-swappable-parameters,-bugprone-narrowing-conversions,-cert-msc32-c,-cert-msc51-cpp,-misc-const-correctness,-misc-include-cleaner,-misc-use-anonymous-namespace,-modernize-use-nullptr,-modernize-avoid-c-arrays,-clang-analyzer-deadcode.DeadStores"
)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(particle-sim PUBLIC OpenMP::OpenMP_CXX)
endif()

target_link_libraries(particle-sim
    PRIVATE
        glfw
        glad
        glm::glm
)

# Copy shaders to build directory
add_custom_command(TARGET particle-sim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/shaders
        ${CMAKE_BINARY_DIR}/shaders
    COMMENT "Copying shaders to build directory"
)

# Platform-specific libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(particle-sim PRIVATE dl pthread)
endif()