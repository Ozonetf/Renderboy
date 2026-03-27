#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define MAX_SHADER_SIZE 4069
int SEED = 0;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

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

GLuint createVertexShader(const char* fileName)
{
    auto VS = std::fopen(fileName, "r");
    if(!VS)
    {
        std::cerr << "cannot open vertex shader "<<fileName<<'\n';
        return 0;
    }
    char VSbuffer[MAX_SHADER_SIZE];
    auto shaderSize = fread(VSbuffer, 1, MAX_SHADER_SIZE-1, VS);
    VSbuffer[shaderSize]='\0';
    int success; char infoLog[512];
    GLuint VShandle = glCreateShader(GL_VERTEX_SHADER);
    const char* VSstr = VSbuffer;
    glShaderSource(VShandle, 1, &VSstr, NULL);
    glCompileShader(VShandle);
    glGetShaderiv(VShandle, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(VShandle, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        // abort();
    }
    return VShandle;
}

GLuint createFragmentShader(const char* fileName)
{
    auto shader = std::fopen(fileName, "r");
    if(!shader)
    {
        std::cerr << "cannot open vertex shader "<<fileName<<'\n';
        return 0;
    }
    char shaderBuffer[MAX_SHADER_SIZE];
    auto shaderSize = fread(shaderBuffer, 1, MAX_SHADER_SIZE-1, shader);
    shaderBuffer[shaderSize]='\0';
    int success; char infoLog[512];
    GLuint shaderhandle = glCreateShader(GL_FRAGMENT_SHADER);
    const char* shaderstr = shaderBuffer;
    glShaderSource(shaderhandle, 1, &shaderstr, NULL);
    glCompileShader(shaderhandle);
    glGetShaderiv(shaderhandle, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderhandle, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        // abort();
    }
    return shaderhandle;
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "MyOpenGLProject", NULL, NULL);
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

    int  success;
    char infoLog[512];
    auto VS = createVertexShader("shaders/Vertex1.glsl");
    auto PS = createFragmentShader("shaders/Frag1.glsl");

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, VS);
    glAttachShader(shaderProgram, PS);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    auto uniformHandle = glGetUniformLocation(shaderProgram, "myfloat");    
    glUseProgram(shaderProgram);
    glDeleteShader(VS);
    glDeleteShader(PS);
    glClearColor( randomFloat(), randomFloat(), randomFloat(), randomFloat() );

    float vertices[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top left 
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    setVA_PC();
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // glClearColor( 0.4f, 0.3f, 0.4f, 0.0f );
        glClear(GL_COLOR_BUFFER_BIT);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderProgram);
        float greenValue = (std::sin(glfwGetTime())+1)/2;
        glUniform1f(uniformHandle, greenValue);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}