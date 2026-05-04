#pragma once
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "main.h"
#include <cassert>
#include <format>
#include <iostream>
#define MAX_SHADER_SIZE 4069
inline GLuint createVertexShader(const char *fileName)
{
    auto VS = std::fopen(fileName, "r");
    if (!VS)
    {
        std::cerr << "cannot open vertex shader " << fileName << '\n';
        return 0;
    }
    char VSbuffer[MAX_SHADER_SIZE];
    auto shaderSize = fread(VSbuffer, 1, MAX_SHADER_SIZE - 1, VS);
    VSbuffer[shaderSize] = '\0';
    GLuint      VShandle = glCreateShader(GL_VERTEX_SHADER);
    const char *VSstr = VSbuffer;
    glShaderSource(VShandle, 1, &VSstr, NULL);
    glCompileShader(VShandle);
    glGetShaderiv(VShandle, GL_COMPILE_STATUS, &GL_SUCC);
    if (!GL_SUCC)
    {
        glGetShaderInfoLog(VShandle, 512, NULL, GL_ERR_INFO);
        std::cerr << std::format("ERROR::SHADER::VERTEX::COMPILATION_FAILED, {}\n{}\n", fileName, GL_ERR_INFO);
        // abort();
    }
    return VShandle;
}

inline GLuint createFragmentShader(const char *fileName)
{
    auto shader = std::fopen(fileName, "r");
    if (!shader)
    {
        std::cerr << "cannot open vertex shader " << fileName << '\n';
        return 0;
    }
    char shaderBuffer[MAX_SHADER_SIZE];
    auto shaderSize = fread(shaderBuffer, 1, MAX_SHADER_SIZE - 1, shader);
    shaderBuffer[shaderSize] = '\0';
    GLuint      shaderhandle = glCreateShader(GL_FRAGMENT_SHADER);
    const char *shaderstr = shaderBuffer;
    glShaderSource(shaderhandle, 1, &shaderstr, NULL);
    glCompileShader(shaderhandle);
    glGetShaderiv(shaderhandle, GL_COMPILE_STATUS, &GL_SUCC);
    if (!GL_SUCC)
    {
        glGetShaderInfoLog(shaderhandle, 512, NULL, GL_ERR_INFO);
        std::cerr << std::format("ERROR::SHADER::VERTEX::COMPILATION_FAILED, {}\n{}\n", fileName, GL_ERR_INFO);
        abort();
    }
    return shaderhandle;
}

using namespace glm;
class shaderProgram
{
  private:
    inline GLint glGetUniformLocationSafe(const char *name)
    {
        auto ret = glGetUniformLocation(m_handle, name);
        // assert(ret != -1 && "INVALID UNIFORM NAME");
        if (ret == -1)
            std::cerr << std::format("SHADER ERROR::INVALID UNIFORM NAME: \"{}\"\n", name);
        return ret;
    }
    GLuint m_handle;

  public:
    shaderProgram() { m_handle = glCreateProgram(); };
    ~shaderProgram() = default;

    void attachVS(const char *filename);
    void attachFS(const char *filename);
    void linkAndActivate();

    inline void   activate() const { glUseProgram(m_handle); };
    inline GLuint handle() const { return m_handle; };

    template <typename T> void setUniform1(const char *uName, T in);
    template <typename T> void setUniform3(const char *uName, T in);

    void setUniformMat4(const char *uName, const glm::mat4 &mat);
    void setUniformMat3(const char *uName, const glm::mat3 &mat);
};

inline void shaderProgram::attachVS(const char *filename)
{
    auto vsHandle = createVertexShader(filename);
    glAttachShader(m_handle, vsHandle);
    glDeleteShader(vsHandle);
}

inline void shaderProgram::attachFS(const char *filename)
{
    auto fsHandle = createFragmentShader(filename);
    glAttachShader(m_handle, fsHandle);
    glDeleteShader(fsHandle);
}

inline void shaderProgram::linkAndActivate()
{
    glLinkProgram(m_handle);
    glGetProgramiv(m_handle, GL_LINK_STATUS, &GL_SUCC);
    if (!GL_SUCC)
    {
        glGetProgramInfoLog(m_handle, 512, NULL, GL_ERR_INFO);
        std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << GL_ERR_INFO << '\n';
    }
    glUseProgram(m_handle);
}

// sets a 4x4 matrix uniform in the shader by name
inline void shaderProgram::setUniformMat4(const char *uName, const glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocationSafe(uName), 1, GL_FALSE, glm::value_ptr(mat));
}

// sets a 4x4 matrix uniform in the shader by name
inline void shaderProgram::setUniformMat3(const char *uName, const glm::mat3 &mat)
{
    glUniformMatrix3fv(glGetUniformLocationSafe(uName), 1, GL_FALSE, glm::value_ptr(mat));
}

template <> inline void shaderProgram::setUniform1<int>(const char *uName, int in)
{
    glUniform1i(glGetUniformLocationSafe(uName), in);
}
template <> inline void shaderProgram::setUniform1<unsigned int>(const char *uName, unsigned int in)
{
    glUniform1ui(glGetUniformLocationSafe(uName), in);
}
template <> inline void shaderProgram::setUniform1<float>(const char *uName, float in)
{
    glUniform1f(glGetUniformLocationSafe(uName), in);
}
template <> inline void shaderProgram::setUniform1<double>(const char *uName, double in)
{
    glUniform1d(glGetUniformLocationSafe(uName), in);
}

template <>
inline void shaderProgram::setUniform3<vec<3, float, defaultp>>(const char *uName, vec<3, float, defaultp> in)
{
    glUniform3fv(glGetUniformLocationSafe(uName), 1, value_ptr(in));
}
