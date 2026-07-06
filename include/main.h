#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

static int _width = 800;
static int _height = 600;

static int  GL_SUCC = 0;
static char GL_ERR_INFO[512]{};

static const char *PROGRAM_NAME = "MyOpenGL";

void glfwErrCallback(int error_code, const char *description);
void winResizeCallback(GLFWwindow *window, int width, int height);

void GLMsgCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const *message,
                   void const *user_param);
