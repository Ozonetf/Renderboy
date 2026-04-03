#pragma once
#include "main.h"
#define MAX_SHADER_SIZE 4069
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
    GLuint VShandle = glCreateShader(GL_VERTEX_SHADER);
    const char* VSstr = VSbuffer;
    glShaderSource(VShandle, 1, &VSstr, NULL);
    glCompileShader(VShandle);
    glGetShaderiv(VShandle, GL_COMPILE_STATUS, &GL_SUCC);
    if(!GL_SUCC)
    {
        glGetShaderInfoLog(VShandle, 512, NULL, GL_ERR_INFO);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << GL_ERR_INFO << std::endl;
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
    GLuint shaderhandle = glCreateShader(GL_FRAGMENT_SHADER);
    const char* shaderstr = shaderBuffer;
    glShaderSource(shaderhandle, 1, &shaderstr, NULL);
    glCompileShader(shaderhandle);
    glGetShaderiv(shaderhandle, GL_COMPILE_STATUS, &GL_SUCC);
    if(!GL_SUCC)
    {  
        glGetShaderInfoLog(shaderhandle, 512, NULL, GL_ERR_INFO);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << GL_ERR_INFO << std::endl;
        // abort();
    }
    return shaderhandle;
}

class shaderProgram
{
private:
    GLuint  m_handle;
    bool    m_valid = false;    //a shader program needs at minimum a vertex and fragment shader attached
public:
    shaderProgram(/* args */);
    ~shaderProgram();

    void attachVS(const char* filename);
    void attachFS(const char* filename);
    void activate();

    inline GLuint handle(){return m_handle;};
    template <typename T>
    void setUniform1(const char* uName, T in);
};

shaderProgram::shaderProgram(/* args */)
{
    m_handle = glCreateProgram();
}

shaderProgram::~shaderProgram()
{
}

void shaderProgram::attachVS(const char* filename)
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

void shaderProgram::activate()
{
    glLinkProgram(m_handle);
    glGetProgramiv(m_handle, GL_LINK_STATUS, &GL_SUCC);
    if(!GL_SUCC) {
        glGetProgramInfoLog(m_handle, 512, NULL, GL_ERR_INFO);
        std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << GL_ERR_INFO << '\n';
    }
    glUseProgram(m_handle);
}

template <>
inline void shaderProgram::setUniform1<int>(const char* uName,int in)
{
    glUniform1i(glGetUniformLocation(m_handle, uName), in);
}
template <>
inline void shaderProgram::setUniform1<unsigned int>(const char* uName,unsigned int in)
{
    glUniform1ui(glGetUniformLocation(m_handle, uName), in);
}
template <>
inline void shaderProgram::setUniform1<float>(const char* uName,float in)
{
    glUniform1f(glGetUniformLocation(m_handle, uName), in);
}
template <>
inline void shaderProgram::setUniform1<double>(const char* uName,double in)
{
    glUniform1d(glGetUniformLocation(m_handle, uName), in);
}