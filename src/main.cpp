#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>

#include "math/math.h"
#include "graphics/shader.h"
#include "graphics/camera.h"
#include "terrain/terrain.h"

// Global variables
Camera camera;
double lastX = 400, lastY = 300;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Mouse callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement((float)xoffset, (float)yoffset);
}

// Scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll((float)yoffset);
}

// Key callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // OpenGL version 3.3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    const int WIDTH = 1280;
    const int HEIGHT = 720;
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D Terrain Generator", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSwapInterval(1); // V-Sync

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set viewport
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Load shaders
    Shader terrainShader;
    terrainShader.compile("src/shaders/terrain.vert", "src/shaders/terrain.frag");

    // Generate terrain
    std::cout << "Generating terrain..." << std::endl;
    Terrain terrain(200, 200, 200.0f, 80.0f);
    terrain.generate();
    std::cout << "Terrain generated successfully!" << std::endl;

    // Initialize camera
    camera = Camera(Vector3(100.0f, 80.0f, 100.0f), Vector3(0.0f, 1.0f, 0.0f));

    // Lighting setup
    Vector3 lightPos(150.0f, 150.0f, 150.0f);
    Vector3 lightColor(1.0f, 1.0f, 1.0f);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input handling
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.processKeyboard(GLFW_KEY_W, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.processKeyboard(GLFW_KEY_S, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.processKeyboard(GLFW_KEY_A, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.processKeyboard(GLFW_KEY_D, deltaTime);

        // Animate light
        float angle = (float)glfwGetTime() * 0.3f;
        lightPos.x = 150.0f * std::cos(angle);
        lightPos.z = 150.0f * std::sin(angle);
        lightPos.y = 100.0f + 50.0f * std::sin(angle * 0.5f);

        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader
        terrainShader.use();

        // Set matrices
        Matrix4 model = Matrix4::identity();
        Matrix4 view = camera.getViewMatrix();
        Matrix4 projection = Matrix4::perspective(
            45.0f * PI / 180.0f,
            (float)WIDTH / (float)HEIGHT,
            0.1f,
            1000.0f
        );

        terrainShader.setMat4("model", model);
        terrainShader.setMat4("view", view);
        terrainShader.setMat4("projection", projection);

        // Set lighting
        terrainShader.setVec3("viewPos", camera.position);
        terrainShader.setVec3("lightPos", lightPos);
        terrainShader.setVec3("lightColor", lightColor);

        // Draw terrain
        terrain.draw();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    terrain.mesh.cleanup();
    glfwTerminate();

    std::cout << "Application closed successfully!" << std::endl;
    return 0;
}
