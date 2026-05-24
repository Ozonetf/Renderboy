#include "game.h"
#include "GLFW/glfw3.h"
#include "Helper.h"
#include "main.h"

#include <format>
int Game::init(GLFWwindow *_window, const int width, const int height)
{
    /* Create a windowed mode window and its OpenGL context */
    m_window = _window;

    m_mouse = mouse{static_cast<double>(width) / 2, static_cast<double>(height) / 2};
    m_mouse.cursorHidden = glfwGetInputMode(m_window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED;
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    m_camera = Camera{static_cast<float>(width), static_cast<float>(height), 90.f, 0.1f, 100.f};
    // Sets all callback function for GLFW
    setCallbacks();
    m_assetManager.init();
    m_assetManager.loadTextures();
    m_assetManager.loadMeshes();
    m_assetManager.getTexture("backpack_albedo").bindToActiveUnit();
    glActiveTexture(GL_TEXTURE1);
    m_assetManager.getTexture("backpack_roughness").bindToActiveUnit();

    // TODO: temp fix for DPI scaling on wayland
    float xScale, yScale;
    glfwGetWindowContentScale(m_window, &xScale, &yScale);
    // NB: m_windowHeight annd Width are the actual pixel count of the window, accounting
    // in DPI scailing per DE
    m_windowHeight = height * yScale;
    m_windowWidth = width * xScale;
    m_camera.updateRatio(m_windowWidth, m_windowHeight);

    m_objects.reserve(10);
    for (int i = 0; i < 10; i++)
    {
        auto ob = GameObject{};
        ob.init();
        // ob.setVertexData(cube);
        ob.translate(glm::vec3(randomFloat(-15, 15), randomFloat(-15, 15), randomFloat(-15, 15)));
        ob.scale(glm::vec3(1));
        ob.m_mesh = &m_assetManager.getMesh("backpack");
        m_objects.push_back(ob);
    }
    m_lightMesh.m_mesh = &m_assetManager.getMesh("sphere2");
    m_lightMesh.scale(glm::vec3(-0.9));

    for (auto i = 0; i < 5; ++i)
    {
        m_pointLights.push_back(
            {.pos = glm::vec3(randomFloat(-7, 7), randomFloat(-7, 7), randomFloat(-7, 7)), .color = randVec3(0, 1)});
    }

    glViewport(0, 0, m_windowWidth, m_windowHeight);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    m_phongShader.attachVS("assets/shaders/BasicVertex.vert");
    m_phongShader.attachFS("assets/shaders/PhongLighting.frag");
    m_phongShader.linkAndActivate();
    m_lightShader.attachVS("assets/shaders/BasicVertex.vert");
    m_lightShader.attachFS("assets/shaders/LightSource.frag");
    m_lightShader.linkAndActivate();
    return 0;
}

void Game::processInput()
{
    constexpr float speed = 0.05f;
    auto            camTranslate = glm::vec3(0.f);
    // auto camRot         = glm::vec3(0.f);

    m_mouse.update(m_window);
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
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
    // if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_PRESS)
    // {
    //     m_flashlight.on = !m_flashlight.on;
    // }
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
    float sinvar = (sin(glfwGetTime()) + 1) / 2;
    for (auto &light : m_pointLights)
    {
        // light.color = glm::vec3(1);
        // light._color = glm::mix(light._pos, -light._pos, sinvar);
    }
    for (auto &ob : m_objects)
    {
        // ob.rotate(glm::vec3(0, 0, 1));
        ob.updateTransform();
    }
    m_flashlight.pos = m_camera.getPos();
    m_flashlight.dir = m_camera.getFront();
    m_flashlight.color = m_flashlight.on ? glm::vec3(1) : glm::vec3(0);
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float sinvar = (sin(glfwGetTime()) + 1) / 2;
    m_lightShader.activate();
    m_lightShader.setUniformMat4("view", m_camera.getView());
    m_lightShader.setUniformMat4("proj", m_camera.getProj());

    // render each light with a simple mesh using m_lightMesh
    for (const auto &pointLight : m_pointLights)
    {
        m_lightShader.setUniform3("lightColor", pointLight.color);

        m_lightMesh.m_pos = pointLight.pos;
        m_lightMesh.updateTransform();
        m_lightShader.setUniformMat4("transform", m_lightMesh.getTransform());
        m_lightMesh.render();
    }

    m_phongShader.activate();
    // m_phongShader.setUniform3("lightColor", color);
    m_phongShader.setUniformMat4("view", m_camera.getView());
    m_phongShader.setUniformMat4("proj", m_camera.getProj());
    m_phongShader.setUniform3("camPos", m_camera.getPos());

    // TODO: refactor this disgusting shit
    m_phongShader.setUniform3("pointLightList[0].pos", m_pointLights.at(0).pos);
    m_phongShader.setUniform3("pointLightList[1].pos", m_pointLights.at(1).pos);
    m_phongShader.setUniform3("pointLightList[2].pos", m_pointLights.at(2).pos);
    m_phongShader.setUniform3("pointLightList[3].pos", m_pointLights.at(3).pos);
    m_phongShader.setUniform3("pointLightList[4].pos", m_pointLights.at(4).pos);
    m_phongShader.setUniform3("pointLightList[0].color", m_pointLights.at(0).color);
    m_phongShader.setUniform3("pointLightList[1].color", m_pointLights.at(1).color);
    m_phongShader.setUniform3("pointLightList[2].color", m_pointLights.at(2).color);
    m_phongShader.setUniform3("pointLightList[3].color", m_pointLights.at(3).color);
    m_phongShader.setUniform3("pointLightList[4].color", m_pointLights.at(3).color);

    m_phongShader.setUniform3("u_flashlight.pos", m_flashlight.pos);
    m_phongShader.setUniform3("u_flashlight.dir", m_flashlight.dir);
    m_phongShader.setUniform1("u_flashlight.intensity", m_flashlight.on ? 1.0f : 0.0f);

    m_phongShader.setUniform1("u_flashlight.outerCutOff", m_flashlight.outerCutOff());
    m_phongShader.setUniform1("u_flashlight.innerCutOff", m_flashlight.innerCutOff());
    // glBindVertexArray(0);
    for (auto &ob : m_objects)
    {
        m_phongShader.setUniformMat4("transform", ob.getTransform());
        m_phongShader.setUniformMat3("normalTransform", ob.getNormalTransform());
        ob.render();
    }
    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);
}

void Game::setCallbacks()
{
    glfwSetFramebufferSizeCallback(m_window, Game::framebufferResizeCallback);
    glfwSetMouseButtonCallback(m_window, Game::mouseButtonCallback);
    glfwSetCursorPosCallback(m_window, Game::mousePosCallback);
    glfwSetKeyCallback(m_window, Game::keyPressCallback);
}

// unusued
void Game::mousePosCallback(GLFWwindow *window, double xpos, double ypos) {}
// unusued
void Game::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {}

void Game::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    instance().m_windowWidth = width;
    instance().m_windowHeight = height;
    instance().m_camera.updateRatio(static_cast<float>(width), static_cast<float>(height));
    glViewport(0, 0, width, height);
}

void Game::keyPressCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS && mods == GLFW_MOD_ALT)
    {
        static bool wireFrame = false;
        wireFrame = !wireFrame;
        std::cerr << std::format("wireframe {}\n", wireFrame ? "on" : "off");
        if (wireFrame)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        instance().m_flashlight.on = !instance().m_flashlight.on;
    }
}
