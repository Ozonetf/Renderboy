#pragma once

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

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

static int GL_SUCC=0;
static char GL_ERR_INFO[512]{};