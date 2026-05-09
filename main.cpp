#include "main.h"
#include "game.h"
#include "shader.h"
#include "texture.h"

#include <GL/gl.h>
#include <format>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

static int _width = 800;
static int _height = 600;

void glfwErrCallback(int error_code, const char *description)
{
    std::cerr << std::format("GLFW error {}::{}\n", error_code, description);
}

int main(void)
{
    // Initialize GLFW
    if (!glfwInit())
        return -1;
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
    glfwMakeContextCurrent(_window);
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    // Create and initialize game
    auto &game = Game::instance();
    if (const auto success = game.init(_window, _width, _height); success == -1)
        return -1;

    float vertices[] = {
        // positions         // colors          // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    stbi_set_flip_vertically_on_load(true);
    auto tex1 = texture{};
    tex1.createFromFile("assets/textures/container2_albedo.png", true);
    tex1.bindToActiveUnit();
    glActiveTexture(GL_TEXTURE1);
    auto tex2 = texture{};
    tex2.createFromFile("assets/textures/container2_shininess.png", true);
    tex2.bindToActiveUnit();

    game.m_phongShader.setUniform1("albedoMap", 0);
    game.m_phongShader.setUniform1("shinenessMap", 1);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // glClearColor(0.4f, 0.3f, 0.4f, 0.0f);
    /* Loop until the user closes the window */
    bool quit = false;
    while (!glfwWindowShouldClose(game.m_window) && quit == false)
    {
        // glBindVertexArray(VertexArrayObject);
        game.update();

        game.render();
        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
