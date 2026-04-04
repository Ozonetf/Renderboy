#include"main.h"
#include "include/shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
int SEED = 0;
static int _width = 2560;
static int _height = 1440;
void framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float randomFloat()
{
    srand(std::time({}) + (SEED++));
    return (float)(rand()) / (float)(RAND_MAX);
}


//sets the vertex attribute to:
//0=position
//1=color
void setVA_PC()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);  
}

//sets the vertex attribute to:
//0=position
//1=color
//2=texCoord
void setVA_PCT()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);  
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);  
}

//sets the vertex attribute to:
//0=position
//2=texCoord
void setVA_PT()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(2);  
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    // specify openGL to use core instead of immediate profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(_width, _height, "MyOpenGLProject", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    auto mySP = shaderProgram{};
    mySP.attachVS("shaders/Vertex1.glsl");
    mySP.attachFS("shaders/Frag1.glsl");
    mySP.activate();
    glClearColor( randomFloat(), randomFloat(), randomFloat(), randomFloat() );

    float vertices[] = {
        // positions         // colors          // texture coords
        0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom left
        -0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f    // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  
    unsigned int EBO, VBO, VAO;
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // setVA_PCT();
    setVA_PT();
    stbi_set_flip_vertically_on_load(true);  
    auto tex1 = texture{};
    tex1.createFromFile("textures/container.jpg", true);  
    tex1.bindToActiveUnit();
    glActiveTexture(GL_TEXTURE1);  
    auto tex2 = texture{};
    tex2.createFromFile("textures/kool.png", true);
    tex2.bindToActiveUnit();
    mySP.setUniform1("myTex", 0);
    mySP.setUniform1("myTex2", 1);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    auto modelTransform = glm::mat4(1.0f);
    auto view = glm::mat4(1.0f);
    auto proj = glm::mat4(1.0f);
    proj = glm::perspective(glm::radians(90.f), ((float)_width / (float)_height), .1f, 100.f);
    view = glm::translate(view, glm::vec3(0, 0, -2.f));
    auto transformLoc = glGetUniformLocation(mySP.handle(), "transform");
    auto projLoc = glGetUniformLocation(mySP.handle(), "proj");
    auto viewLoc = glGetUniformLocation(mySP.handle(), "view");
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // glClearColor( 0.4f, 0.3f, 0.4f, 0.0f );
        glClear(GL_COLOR_BUFFER_BIT);
        float sinf = (std::sin(glfwGetTime())+1)/160;
        mySP.setUniform1("myfloat", sinf);
        modelTransform = glm::rotate(modelTransform, sinf, glm::vec3(0.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelTransform));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glBindVertexArray(VAO);

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}