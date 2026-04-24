#include "game.h"
#include <format>
#include <iostream>
Game::Game() : m_mouse()
{
}

int Game::init(const int width, const int height)
{
    /* Create a windowed mode window and its OpenGL context */
    m_window = glfwCreateWindow(width, height, PROGRAM_NAME, nullptr, nullptr);
    m_mouse = mouse{static_cast<double>(width) / 2, static_cast<double>(height) / 2};
    m_camera = Camera{static_cast<float>(width), static_cast<float>(height), 90.f, 0.1f, 100.f};
    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    m_mouse.cursorHidden = glfwGetInputMode(m_window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED;
    glfwSetFramebufferSizeCallback(m_window, Game::framebufferResizeCallback);
    glfwSetMouseButtonCallback(m_window, Game::mouseButtonCallback);
    glfwSetCursorPosCallback(m_window, Game::mousePosCallback);

    glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    m_windowHeight = height;
    m_windowWidth = width;
    return 0;
}

void Game::processInput()
{
    constexpr float speed = 0.05f;
    auto            camTranslate = glm::vec3(0.f);
    // auto camRot         = glm::vec3(0.f);

    m_mouse.update(m_window);
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camTranslate.z -= speed;
    }
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camTranslate.x -= speed;
    }
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camTranslate.z += speed;
    }
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camTranslate.x += speed;
    }
    if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camTranslate.y += speed;
    }
    if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camTranslate.y -= speed;
    }
    m_camera.transformCamFPS(camTranslate, -m_mouse.dypos * m_mouse.sens, m_mouse.dxpos * m_mouse.sens);
}

void Game::update()
{
    const auto curTime = glfwGetTime();
    m_deltaTime = curTime - m_lastFrame;
    m_lastFrame = curTime;
    processInput();
    if (const int newTime = static_cast<int>(curTime); newTime != m_curTime)
    {
        m_curTime = newTime;
        glfwSetWindowTitle(m_window, std::format("{} FPS: {}", PROGRAM_NAME, (1.0f / m_deltaTime)).c_str());
    }
}

void Game::render()
{
    if (!inited)
    {
        inited = true;
        instance().framebufferResizeCallback(m_window, m_windowWidth, m_windowHeight);
    }
    return;
}

// unusued
void Game::mousePosCallback(GLFWwindow *window, double xpos, double ypos)
{
}
// unusued
void Game::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
}

void Game::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    instance().m_windowWidth = width;
    instance().m_windowHeight = height;
    instance().m_camera.updateRatio(static_cast<float>(width), static_cast<float>(height));
    glViewport(0, 0, width, height);
}
