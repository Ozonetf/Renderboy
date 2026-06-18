#include "main.h"
#include "Game.hpp"
#include "Helper.hpp"
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

// GL error callback, credit: https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions
void GLMsgCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const *message,
                   void const *user_param)
{
    auto const src_str = [source]() {
        switch (source)
        {
        default:
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
            return "OTHER";
        }
    }();

    auto const type_str = [type]() {
        switch (type)
        {
        default:
        case GL_DEBUG_TYPE_ERROR:
            return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER:
            return "MARKER";
        case GL_DEBUG_TYPE_OTHER:
            return "OTHER";
        }
    }();

    auto const severity_str = [severity]() {
        switch (severity)
        {
        default:
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
        case GL_DEBUG_SEVERITY_LOW:
            return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH:
            return "HIGH";
        }
    }();
    std::cerr << "\033[31m" << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message
              << "\033[0m\n";
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
    glEnable(GL_DEBUG_OUTPUT);
    // generally dont care about notifications for error callback
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback(GLMsgCallback, nullptr);

    // Create and initialize game
    auto &game = Game::instance();
    if (const auto success = game.init(_window, _width, _height); success == -1)
        return -1;

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
