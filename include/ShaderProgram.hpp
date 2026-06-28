#pragma once
#include "Geometry.hpp"
#include "Helper.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "main.h"
#include <cassert>
#include <cstddef>
#include <glad/glad.h>
#include <iostream>

using namespace glm;
class ShaderProgram
{
  private:
    inline GLint glGetUniformLocationSafe(const char *name)
    {
        auto ret = glGetUniformLocation(m_handle, name);
        // assert(ret != -1 && "INVALID UNIFORM NAME");
        if (ret == -1)
            logToCerr("SHADER ERROR::INVALID UNIFORM NAME: \"{}\"\n", name);
        return ret;
    }

  public:
    ShaderProgram() : m_handle(glCreateProgram()) {};
    ~ShaderProgram() = default;

    inline void activate() const { glUseProgram(m_handle); };
    inline void attach(GLuint shader) const { glAttachShader(m_handle, shader); };

    void        compile() { compile(this->m_handle); };
    static void compile(GLuint program);

    static void reload(GLuint program);

    template <typename T>
    void setUniform1(const char *uName, T in);
    template <typename T>
    void setUniform3(const char *uName, T in);
    template <typename T>
    void setUniform3(const char *uName, T in, size_t count);

    void setUniformMat4(const char *uName, const glm::mat4 &mat);
    void setUniformMat3(const char *uName, const glm::mat3 &mat);

    const GLuint m_handle;
};

inline void ShaderProgram::compile(GLuint program)
{
    // static GLuint  attachedShaders[5];
    // static GLsizei count;
    // glGetAttachedShaders(program, sizeof(attachedShaders), &count, attachedShaders);
    // assert(count < 5);
    // for (GLuint i = 0; i < count; ++i)
    // {
    //     glAttachShader(program, attachedShaders[i]);
    // }
    // glattachshader(program, vhandle);
    // glattachshader(program, fhandle);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &GL_SUCC);
    if (!GL_SUCC)
    {
        glGetProgramInfoLog(program, 512, NULL, GL_ERR_INFO);
        std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << GL_ERR_INFO << '\n';
    }
    else
        glUseProgram(program);

    // glDetachShader(program, vHandle);
    // glDetachShader(program, fHandle);
}

// Triggered when an attached shader is updated(hot reload), reattache all shaders,
// link program then detach them. Job of deleting shader object is handled somewhere else
inline void ShaderProgram::reload(GLuint program)
{
    compile(program);
    // for (int i = 0; i < count; ++i)
    // {
    //     glDetachShader(program, attachedHandles[i]);
    // }
}
// sets a 4x4 matrix uniform in the shader by name
inline void ShaderProgram::setUniformMat4(const char *uName, const glm::mat4 &mat)
{
    glProgramUniformMatrix4fv(m_handle, glGetUniformLocationSafe(uName), 1, GL_FALSE, glm::value_ptr(mat));
}

// sets a 4x4 matrix uniform in the shader by name
inline void ShaderProgram::setUniformMat3(const char *uName, const glm::mat3 &mat)
{
    glProgramUniformMatrix3fv(m_handle, glGetUniformLocationSafe(uName), 1, GL_FALSE, glm::value_ptr(mat));
}

template <>
inline void ShaderProgram::setUniform1<int>(const char *uName, int in)
{
    glProgramUniform1i(m_handle, glGetUniformLocationSafe(uName), in);
}
template <>
inline void ShaderProgram::setUniform1<unsigned int>(const char *uName, unsigned int in)
{
    glProgramUniform1ui(m_handle, glGetUniformLocationSafe(uName), in);
}
template <>
inline void ShaderProgram::setUniform1<float>(const char *uName, float in)
{
    glProgramUniform1f(m_handle, glGetUniformLocationSafe(uName), in);
}
template <>
inline void ShaderProgram::setUniform1<double>(const char *uName, double in)
{
    glProgramUniform1d(m_handle, glGetUniformLocationSafe(uName), in);
}

template <>
inline void ShaderProgram::setUniform3<vec<3, float, defaultp>>(const char *uName, vec<3, float, defaultp> in)
{
    glProgramUniform3fv(m_handle, glGetUniformLocationSafe(uName), 1, value_ptr(in));
}

template <>
inline void ShaderProgram::setUniform3<vec<3, float, defaultp>>(const char *uName, vec<3, float, defaultp> in,
                                                                size_t count)
{
    glProgramUniform3fv(m_handle, glGetUniformLocationSafe(uName), count, value_ptr(in));
}
