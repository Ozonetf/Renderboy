#include "game.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "main.h"
#include <format>

Game::Game() : m_mouse()
{
}

int Game::init(GLFWwindow *_window, const int width, const int height)
{
    /* Create a windowed mode window and its OpenGL context */
    m_window = _window;
    m_mouse = mouse{static_cast<double>(width) / 2, static_cast<double>(height) / 2};
    m_camera = Camera{static_cast<float>(width), static_cast<float>(height), 90.f, 0.1f, 100.f};
    /* Make the window's context current */
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    m_mouse.cursorHidden = glfwGetInputMode(m_window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED;
    glfwSetFramebufferSizeCallback(m_window, Game::framebufferResizeCallback);
    glfwSetMouseButtonCallback(m_window, Game::mouseButtonCallback);
    glfwSetCursorPosCallback(m_window, Game::mousePosCallback);

    glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    // TODO: temp fix for DPI scaling on wayland
    float xScale, yScale;
    glfwGetWindowContentScale(m_window, &xScale, &yScale);
    m_windowHeight = height * yScale;
    m_windowWidth = width * xScale;
    m_camera.updateRatio(m_windowWidth, m_windowHeight);
    // std::cerr << std::format("w: {}, h: {}", m_windowWidth, m_windowHeight);
    m_objects.reserve(10);
    for (int i = 0; i < 10; i++)
    {
        auto ob = GameObject{};
        ob.init();
        ob.setVertexData(cube);
        ob.translate(glm::vec3(randomFloat(-30, 30), randomFloat(-30, 30), randomFloat(-30, 30)));
        ob.scale(glm::vec3(randomFloat(1.0f, 5.0f), randomFloat(1.0f, 5.0f), randomFloat(1.0f, 5.0f)));
        m_objects.push_back(ob);
    }

    m_shader.attachVS("assets/shaders/Vertex1.glsl");
    m_shader.attachFS("assets/shaders/Frag1.glsl");
    m_shader.activate();
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
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
    // update fps once every second
    if (const int newTime = static_cast<int>(curTime); newTime != m_curTime)
    {
        m_curTime = newTime;
        glfwSetWindowTitle(m_window, std::format("{} FPS: {}", PROGRAM_NAME, (1.0f / m_deltaTime)).c_str());
    }
    processInput();
    float sinf = (std::sin(glfwGetTime()) + 1);
    for (auto &ob : m_objects)
    {
        ob.rotate(glm::vec3(0, 0, sinf));
        ob.updateTransform();
    }
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shader.setUniformMat4("view", m_camera.getView());
    m_shader.setUniformMat4("proj", m_camera.getProj());
    // glBindVertexArray(0);
    for (auto &ob : m_objects)
    {
        m_shader.setUniformMat4("transform", ob.getTransform());
        ob.render();
    }
    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);
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
