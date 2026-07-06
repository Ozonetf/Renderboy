#pragma once

#include "main.h"
#include <filesystem>

#define MAX_SHADER_SIZE 4069
enum ShaderType
{
    none = 0,
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER,
    geometry = GL_GEOMETRY_SHADER
};

// Gets the string of shader type
inline std::string stStr(ShaderType T)
{
    switch (T)
    {
    default:
    case none:
        return "UNINITILIZED SHADER";
    case vertex:
        return "VERTEX";
    case fragment:
        return "FRAGMENT";
    case geometry:
        return "GEOMETRY";
    };
}

class ShaderFile
{
    GLuint                          handle = 0;
    ShaderType                      type = none;
    std::filesystem::file_time_type cachedLastWrite{};

  public:
    ShaderFile() = delete;
    ShaderFile(std::filesystem::directory_entry ent) : m_dirEntry(ent) {};

    inline void init(ShaderType type)
    {
        this->cachedLastWrite = m_dirEntry.last_write_time();
        this->type = type;
    }

    GLuint      requestHandle();
    GLuint      compile();
    GLuint      reload(std::filesystem::file_time_type newTime);
    inline auto getCachedWriteTime() const { return this->cachedLastWrite; };
    inline void cleanup();

    std::filesystem::directory_entry getdir() const { return this->m_dirEntry; };

    const std::filesystem::directory_entry m_dirEntry;
};
