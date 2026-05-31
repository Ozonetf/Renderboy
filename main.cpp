#include "main.h"
#include "Helper.h"
#include "game.h"
#include "shader.h"
#include <format>
#include <iostream>

static int _width = 800;
static int _height = 600;

void glfwErrCallback(int error_code, const char *description)
{
    std::cerr << std::format("GLFW error {}::{}\n", error_code, description);
}

void winResizeCallback(GLFWwindow *window, int width, int height)
{
    std::cerr << std::format("x: {} y: {}\n", width, height);
}

int main(void)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "GLFW innitilization failed";
        return -1;
    }
    // specify openGL to use core instead of immediate profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CENTER_CURSOR, GL_TRUE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_FALSE);

    // set up error callback function for GLFW, any errors thrown by GLFW
    // will log to std error
    glfwSetErrorCallback(glfwErrCallback);
    // We need to create a context first before we can start
    // calling GLAD/GL functions
    auto _window = glfwCreateWindow(_width, _height, PROGRAM_NAME, nullptr, nullptr);
    if (!_window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(_window);
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    // Create and initialize game
    auto &game = Game::instance();
    if (const auto success = game.init(_window, _width, _height); success == -1)
        return -1;

    game.m_phongShader->setUniform1("albedoMap", 0);
    game.m_phongShader->setUniform1("shinenessMap", 1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    {
        // Window desc
        std::cerr << std::format("Display description:\n");
        auto monitor = glfwGetPrimaryMonitor();
        int  px, py;
        glfwGetMonitorPhysicalSize(monitor, &px, &py);
        logToCerr("Display:{}\nDimensions x: {}mm y:{}mm\n", glfwGetMonitorName(monitor), px, py);
        float fx, fy;
        glfwGetMonitorContentScale(monitor, &fx, &fy);
        std::cerr << std::format("Monitor scale x: {} y:{}\n", fx, fy);
        glfwGetWindowContentScale(_window, &fx, &fy);
        std::cerr << std::format("window scale x: {} y:{}\n", fx, fy);
    }

    // glClearColor(0.4f, 0.3f, 0.4f, 0.0f);
    /* Loop until the user closes the window */
    bool quit = false;
    while (!glfwWindowShouldClose(game.m_window) && quit == false)
    {
        game.update();
        game.render();
        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
