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
    m_window = glfwCreateWindow(width, height, PROGRAM_NAME, nullptr, nullptr);
    // glfwGetCursorPos()
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
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    m_mouse.cursorHidden = glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED? false:true;
    glfwSetFramebufferSizeCallback(m_window, Game::framebufferResizeCallback);    
    glfwSetMouseButtonCallback(m_window, Game::mouseButtonCallback);
    glfwSetCursorPosCallback(m_window, Game::mousePosCallback);

    // glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    return 0;
}


void Game::processInput()
{    
    constexpr float speed = 0.05f;
    auto camTranslate   = glm::vec3(0.f);
    auto camRot         = glm::vec3(0.f);

    m_mouse.update(m_window);
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
    m_camera.rotatePitchYaw(m_mouse.dypos, m_mouse.dxpos);
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
        m_curTime = newTime;
        glfwSetWindowTitle(m_window, std::format("{} FPS: {}",PROGRAM_NAME, (1.0f / m_deltaTime)).c_str());
    }
}

void Game::render()
{
    return;
}

void Game::mousePosCallback(GLFWwindow *window, double xpos, double ypos)
{
    // std::cout<<std::format("DX: {}  DY: {}\n", xpos, ypos);
    return;
    // instance().m_mouse._dxpos = instance().m_mouse._lastxpos - xpos; instance().m_mouse._dypos = instance().m_mouse._lastypos - ypos;
    // instance().m_mouse._lastxpos = xpos;
    // instance().m_mouse._lastypos = ypos;
    
    // std::cout<<std::format("dx: {} dy: {}\n", instance().m_mouse._dxpos, instance().m_mouse._dypos);

    // instance().m_camera.rotatePitchYaw(instance().m_mouse._dxpos, instance().m_mouse._dypos);
}

void Game::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    // if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    // {
    //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //     std::cout<<"pressed\n";
    // }
    // if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    // {
    //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //     std::cout<<"released\n";
    // }
}
void Game::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    instance().m_windowWidth = width;
    instance().m_windowHeight = height;
    glViewport(0, 0, width, height);
}