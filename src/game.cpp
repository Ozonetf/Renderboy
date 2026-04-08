#include "game.h"
Game::Game()
{
    // m_window = std::make_unique<GLFWwindow>();
}

Game::~Game()
{
}

int Game::init(int width, int height)
{
    m_window = glfwCreateWindow(width, height, "myprogram", nullptr, nullptr);
    m_mouse = mouse{static_cast<double>(width/2), static_cast<double>(height/2)};

    /* Create a windowed mode window and its OpenGL context */
    // GLFWwindow* window = glfwCreateWindow(_width, _height, "MyOpenGLProject", NULL, NULL);
    if (!m_window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);
    glfwSetCursorPos(m_window, m_mouse._lastxpos, m_mouse._lastypos);

    glfwSetFramebufferSizeCallback(m_window, Game::framebufferResizeCallback);
    
    glfwSetMouseButtonCallback(m_window, Game::mouseButtonCallback);
    glfwSetCursorPosCallback(m_window, Game::mousePosCallback);
    return 0;
}


void Game::processInput()
{    
    constexpr float speed = 0.05f;
    auto camTranslate   = glm::vec3(0.f);
    auto camRot         = glm::vec3(0.f);
    // camRot.y = _dxpos;
    // camRot.x = _dypos;
    if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camTranslate.z -= speed;
    }        
    if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camTranslate.x += speed;
    }    
    if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camTranslate.z += speed;        
    }    
    if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {   
        camTranslate.x -= speed;
    }    
    m_camera.transformCamFPS(camTranslate, 0, 0);
}

void Game::update()
{
    auto curTime = glfwGetTime();
    m_deltaTime = curTime - m_lastFrame;
    m_lastFrame = curTime;
    processInput();
    int newTime = static_cast<int>(curTime);
    if(newTime != m_curTime)
    {
        std::cout <<std::format("{} FPS: {}\n", newTime, (1.0f / m_deltaTime));
        m_curTime = newTime;
        glfwSetWindowTitle(m_window, std::format("myprogram FPS: {}", (1.0f / m_deltaTime)).c_str());
    }
}

void Game::render()
{
    return;
}

void Game::mousePosCallback(GLFWwindow *window, double xpos, double ypos)
{
    instance().m_mouse._dxpos = instance().m_mouse._lastxpos - xpos; instance().m_mouse._dypos = instance().m_mouse._lastypos - ypos;
    instance().m_mouse._lastxpos = xpos;
    instance().m_mouse._lastypos = ypos;
}

void Game::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        std::cout<<"pressed\n";
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        std::cout<<"released\n";
    }
}
void Game::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    instance().m_windowWidth = width;
    instance().m_windowHeight = height;
    glViewport(0, 0, width, height);
}