#pragma once
#include "AssetManager.hpp"
#include "GameObject.h"
#include "camera.h"
#include "shader.h"
#include <vector>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
using namespace glm;
struct PointLight
{
    vec3  pos = vec3(0);
    vec3  color = vec3(1);
    float constant = 1;
    float linear = 0.14f;
    float quadratic = 0.07f;
};

struct SpotLight
{
    vec3 pos = vec3(0);
    vec3 dir = vec3(0);
    vec3 color = vec3(1);
    // _outer is the angle from the middle of light ray
    // to the cut off angle IN DEGREES
    float outer = 45 / 2.f;
    float inner = 30 / 2.f;
    bool  on = false;

    inline float outerCutOff() { return glm::cos(glm::radians(outer)); };
    inline float innerCutOff() { return glm::cos(glm::radians(inner)); };
};

struct mouse
{
    double dxpos = 0;
    double dypos = 0;
    double lastxpos = dypos;
    double lastypos = dypos;
    double sens = 0.2;
    bool   cursorHidden = false;

    inline void update(GLFWwindow *window)
    {
        // if right mouse buttone is pressed, diable the cursor
        bool rclick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS ? true : false;
        if (rclick)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            double curX, curY;
            glfwGetCursorPos(window, &curX, &curY);
            // GLFW reports drastic different cursor pos depending on if input mode is
            // GLFW_CURSOR_DISABLED or NORMAL for some reason, therefore changing the
            // mode will report a huge dx/dy for 1 tick, this is to prevent that.
            if (cursorHidden != rclick)
            {
                cursorHidden = rclick;
                dxpos = 0;
                dypos = 0;
            }
            else
            {
                dxpos = curX - lastxpos;
                dypos = curY - lastypos;
            }
            lastxpos = curX;
            lastypos = curY;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorHidden = rclick;
            dxpos = 0;
            dypos = 0;
        }
    }
};

class Game
{
  private:
    Game() = default;
    void processInput();

    int   m_windowWidth;
    int   m_windowHeight;
    int   m_curTime = 0;
    float m_deltaTime = 0.0f; // Time between current frame and last frame
    float m_lastFrame = 0.0f; // Time of last frame

    mouse        m_mouse;
    AssetManager m_assetManager;
    Camera       m_camera;
    GameObject   m_lightMesh;
    SpotLight    m_flashlight;

    std::vector<GameObject> m_objects{};
    std::vector<PointLight> m_pointLights{};

  public:
    static Game &instance()
    {
        static Game gameInstance{};
        return gameInstance;
    }
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    int  init(GLFWwindow *_window, int width, int height);
    void update();
    void render();

    inline Camera getCamera() const { return m_camera; };

    // Callback Functions
    void        setCallbacks();
    static void mousePosCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
    static void keyPressCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    shaderProgram m_phongShader;
    shaderProgram m_lightShader;
    GLFWwindow   *m_window = nullptr;
};
