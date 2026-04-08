#pragma once
#include "main.h"
#include "camera.h"
struct mouse
{
    double _dxpos = 0;
    double _dypos = 0;
    double _lastxpos = _dypos;
    double _lastypos = _dypos;
    mouse(){};
    mouse(double x, double y)
    {
        _lastxpos = x; _lastypos = y;
    }
};


class Game
{
private:
    Game();
    void processInput();

    int m_windowWidth;
    int m_windowHeight;
    int m_curTime = 0;
    float m_deltaTime = 0.0f;	// Time between current frame and last frame
    float m_lastFrame = 0.0f; // Time of last frame

    mouse   m_mouse;
    Camera  m_camera;
public:
    static Game& instance()
    {
        static Game gameInstance{};
        return gameInstance;
    }
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    ~Game();

    int init(int width, int height);
    void update();
    void render();
    inline Camera getCamera () const{return m_camera;};
    static void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    
    GLFWwindow* m_window;

};