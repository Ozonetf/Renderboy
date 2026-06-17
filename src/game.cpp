#include "game.h"
#include "GLFW/glfw3.h"
#include "Helper.h"
#include "main.h"

#include <format>
#include <glad/glad.h>

void Game::tempFrameBufferSetUp()
{
    glGenFramebuffers(1, &this->FBO);
    // GL_READ_FRAMEBUFFER or GL_DRAW_FRAMEBUFFER If a framebuffer object is bound to GL_DRAW_FRAMEBUFFER or
    // GL_READ_FRAMEBUFFER, it becomes the target for rendering or readback operations, respectively, until it is
    // deleted or another framebuffer is bound to the corresponding bind point.
    // GL_FRAMBUFFER is both read and write.
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    // bind a texture as color attachment for frame buffer
    frameBufferTex.createEmpty(this->m_windowWidth, this->m_windowHeight);
    // set texture as frame buffer color attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTex.handle(), 0);
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frameBufferTex.handle(), 0);

    // bind a render buffer as depth+stencil buffer
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->m_windowWidth, this->m_windowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        abort();
    // unbind render buffer after setup
    // 0 sets frame buffer to default
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

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
    m_assetManager.loadShaderFiles();

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
    m_phongShader = m_assetManager.createShaderProgram("phongShader", "BasicVertex.vert", "PhongLighting.frag");
    m_lightShader = m_assetManager.createShaderProgram("lightShader", "BasicVertex.vert", "LightSource.frag");
    m_skyboxShader = m_assetManager.createShaderProgram("skybox", "skybox.vert", "skybox.frag");

    m_assetManager.getTexture("backpack_albedo").bindToActiveUnit();
    glActiveTexture(GL_TEXTURE1);
    m_assetManager.getTexture("backpack_roughness").bindToActiveUnit();

    m_phongShader->setUniform1("albedoMap", 0);
    m_phongShader->setUniform1("shinenessMap", 1);
    m_postProcessShader = m_assetManager.createShaderProgram("postProcess", "post_process.vert", "post_process.frag");

    tempFrameBufferSetUp();
    glGenVertexArrays(1, &this->m_quadVAO);
    GLuint tempvbo;
    glGenBuffers(1, &tempvbo);
    glBindVertexArray(m_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, tempvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, &viewportQuad, GL_STATIC_DRAW);
    setVA_PT2();
    glBindVertexArray(0);

    m_cubemap.createCubeMap();
    glGenVertexArrays(1, &this->skyboxVAO);
    GLuint skyVBO;
    glCreateBuffers(1, &skyVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
        ob.rotate(glm::vec3(0, 0, 1));
        ob.updateTransform();
    }
    m_flashlight.pos = m_camera.getPos();
    m_flashlight.dir = m_camera.getFront();
    m_flashlight.color = m_flashlight.on ? glm::vec3(1) : glm::vec3(0);
}

void Game::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // since we're setting the pixel depth to 1 in the vert shader
    // lequal is used instead of less in order to not discard visible skybox pixels
    glDepthFunc(GL_LEQUAL);
    m_skyboxShader->activate();
    m_skyboxShader->setUniform1("skybox", 0);
    m_skyboxShader->setUniformMat4("view", glm::mat4(glm::mat3(m_camera.getView())));
    m_skyboxShader->setUniformMat4("projection", m_camera.getProj());
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    m_cubemap.bindToActiveUnit(GL_TEXTURE_CUBE_MAP);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);

    float sinvar = (sin(glfwGetTime()) + 1) / 2;

    m_lightShader->activate();
    m_lightShader->setUniformMat4("view", m_camera.getView());
    m_lightShader->setUniformMat4("proj", m_camera.getProj());

    // render each light with a simple mesh using m_lightMesh
    for (const auto &pointLight : m_pointLights)
    {
        m_lightShader->setUniform3("lightColor", pointLight.color);

        m_lightMesh.m_pos = pointLight.pos;
        m_lightMesh.updateTransform();
        m_lightShader->setUniformMat4("transform", m_lightMesh.getTransform());
        m_lightMesh.render();
    }

    m_phongShader->activate();
    m_phongShader->setUniformMat4("view", m_camera.getView());
    m_phongShader->setUniformMat4("proj", m_camera.getProj());
    m_phongShader->setUniform3("camPos", m_camera.getPos());

    glActiveTexture(GL_TEXTURE0);
    m_assetManager.getTexture("backpack_albedo").bindToActiveUnit();
    glActiveTexture(GL_TEXTURE1);
    m_assetManager.getTexture("backpack_roughness").bindToActiveUnit();
    m_phongShader->setUniform1("albedoMap", 0);
    m_phongShader->setUniform1("shinenessMap", 1);

    // TODO: refactor this disgusting shit
    m_phongShader->setUniform3("pointLightList[0].pos", m_pointLights.at(0).pos);
    m_phongShader->setUniform3("pointLightList[1].pos", m_pointLights.at(1).pos);
    m_phongShader->setUniform3("pointLightList[2].pos", m_pointLights.at(2).pos);
    m_phongShader->setUniform3("pointLightList[3].pos", m_pointLights.at(3).pos);
    m_phongShader->setUniform3("pointLightList[4].pos", m_pointLights.at(4).pos);
    m_phongShader->setUniform3("pointLightList[0].color", m_pointLights.at(0).color);
    m_phongShader->setUniform3("pointLightList[1].color", m_pointLights.at(1).color);
    m_phongShader->setUniform3("pointLightList[2].color", m_pointLights.at(2).color);
    m_phongShader->setUniform3("pointLightList[3].color", m_pointLights.at(3).color);
    m_phongShader->setUniform3("pointLightList[4].color", m_pointLights.at(4).color);

    m_phongShader->setUniform3("u_flashlight.pos", m_flashlight.pos);
    m_phongShader->setUniform3("u_flashlight.dir", m_flashlight.dir);
    m_phongShader->setUniform1("u_flashlight.intensity", m_flashlight.on ? 1.0f : 0.0f);

    m_phongShader->setUniform1("u_flashlight.outerCutOff", m_flashlight.outerCutOff());
    m_phongShader->setUniform1("u_flashlight.innerCutOff", m_flashlight.innerCutOff());
    for (auto &ob : m_objects)
    {
        m_phongShader->setUniformMat4("transform", ob.getTransform());
        m_phongShader->setUniformMat3("normalTransform", ob.getNormalTransform());
        ob.render();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_postProcessShader->activate();
    m_postProcessShader->setUniform1("screenTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    frameBufferTex.bindToActiveUnit();
    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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
    instance().frameBufferTex.createEmpty(width, height);
    // TODO: update size-dependent resources
    glBindRenderbuffer(GL_RENDERBUFFER, instance().FBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
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
    if (key == GLFW_KEY_R && action == GLFW_PRESS && mods == GLFW_MOD_ALT)
    {
        instance().m_assetManager.updateShaders();
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        instance().m_flashlight.on = !instance().m_flashlight.on;
    }
}
