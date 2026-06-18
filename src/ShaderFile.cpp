#include "ShaderFile.hpp"
#include "Helper.hpp"
#include "main.h"

// Returns the shader object, compiles if havent been, caches
// the program for future updates(hot reload)
GLuint ShaderFile::requestHandle()
{
    // this->programRefs.push_back(program);
    // glIsShader returns false if program object is not valid or
    // marked for deletion, in which cas we recompile/recreate shader object
    if (glIsShader(this->handle) != GL_TRUE)
        return this->compile();
    else
        return this->handle;
}

// Compiles shader then returns GL handle, returns empty
// handle (0) if proc failed
GLuint ShaderFile::compile()
{
    const auto  pathStr = this->m_dirEntry.path().string();
    const char *fileName = pathStr.c_str();
    auto        shaderSource = std::fopen(fileName, "r");
    if (!shaderSource)
    {
        std::cerr << "cannot open vertex shader " << fileName << '\n';
        return 0;
    }
    // TODO: static here?
    static char shaderBuffer[MAX_SHADER_SIZE];
    // read size is MAX_SHADER_SIZE - 1 because need 1 terminating char '\0' at the end
    auto shaderSize = fread(shaderBuffer, 1, MAX_SHADER_SIZE - 1, shaderSource);
    assert(shaderSize < MAX_SHADER_SIZE - 1);
    shaderBuffer[shaderSize] = '\0';
    // First time compilation
    if (handle == 0)
    {
        assert(this->type != ShaderType::none);
        handle = glCreateShader(this->type);
    }
    logToCerr("Compiling shader {}\n", pathStr);
    const char *shaderBufferPtr = shaderBuffer;
    glShaderSource(this->handle, 1, &shaderBufferPtr, NULL);
    glCompileShader(this->handle);
    glGetShaderiv(this->handle, GL_COMPILE_STATUS, &GL_SUCC);
    if (!GL_SUCC)
    {
        glGetShaderInfoLog(this->handle, 512, NULL, GL_ERR_INFO);
        logToCerr("ERROR::{}::SHADER::COMPILATION_FAILED, {}\n{}\n", stStr(this->type), fileName, GL_ERR_INFO);
        return 0;
    }
    return this->handle;
}

// Flags shader for deletion and invalidates internal handle, all referenceing
// programs should detach shaderfiles after linking. Since the internal handle
// is invalidated, any subsequent use of shaderfile will trigger a recompilation.
inline void ShaderFile::cleanup()
{
    assert(glIsShader(this->handle) == GL_TRUE);
    glDeleteShader(this->handle);
    this->handle = 0;
}

GLuint ShaderFile::reload(std::filesystem::file_time_type modTime)
{
    this->lastWriteTime = modTime;
    return compile();
    // for (const auto program : programRefs)
    // {
    //     shaderProgram::reload(program);
    // }
}
