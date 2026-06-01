#pragma once

#include "main.h"
#include <filesystem>

#define MAX_SHADER_SIZE 4069
enum ShaderType
{
    none = 0,
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER
};

// Gets the string of shader type
inline std::string stStr(ShaderType T)
{
    switch (T)
    {
    case vertex:
        return "VERTEX";
    case fragment:
        return "FRAGMENT";
    case none:
        return "UNINITILIZED SHADER";
    };
}

class ShaderFile
{
    bool                            modified = false;
    GLuint                          handle = 0;
    ShaderType                      type = none;
    std::filesystem::file_time_type lastWriteTime{};

  public:
    ShaderFile() = delete;
    ShaderFile(std::filesystem::directory_entry ent) : m_dirEntry(ent) {};

    inline void init(std::filesystem::directory_entry file, ShaderType type)
    {
        this->lastWriteTime = file.last_write_time();
        this->type = type;
    }

    GLuint      requestHandle();
    GLuint      compile();
    GLuint      reload(std::filesystem::file_time_type modTime);
    inline auto lastWrite() const { return this->lastWriteTime; };
    inline void cleanup();

    std::filesystem::directory_entry getdir() const { return this->m_dirEntry; };

    const std::filesystem::directory_entry m_dirEntry;
};
