cmake_minimum_required(VERSION 3.10)
project(TerrainGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)

set(SOURCES
    src/main.cpp
    src/math/math.cpp
    src/graphics/shader.cpp
    src/graphics/mesh.cpp
    src/graphics/camera.cpp
    src/terrain/terrain.cpp
    src/terrain/perlin_noise.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    OpenGL::OpenGL
    GLFW::glfw3
    GLEW::GLEW
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
