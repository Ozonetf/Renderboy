#pragma once
#include "main.h"
#include "camera.h"
struct mouse
{
    double  dxpos = 0;
    double  dypos = 0;
    double  lastxpos = dypos;
    double  lastypos = dypos;
    double  sens = 0.2;
    bool    cursorHidden = false;

    mouse(){};
    mouse(double x, double y)
    {
        lastxpos = x; lastypos = y;
    }
    inline void update(GLFWwindow *window)
    {
        //if right mouse buttone is pressed, diable the cursor
        bool rclick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS ? true : false;
        if(rclick)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            double curX, curY;
            glfwGetCursorPos(window, &curX, &curY);
            // GLFW reports drastic different cursor pos depending on if input mode is
            // GLFW_CURSOR_DISABLED or NORMAL for some reason, therefore changing the
            // mode will report a huge dx/dy for 1 tick, this is to prevent that.
            if(cursorHidden != rclick)
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
            lastxpos = curX; lastypos = curY;            
        }  
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorHidden = rclick;   
            dxpos = 0;
            dypos = 0; 
        }
        // std::cout<<std::format("Dx: {}      Dy: {}\n", floor(dxpos), floor(dypos));
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