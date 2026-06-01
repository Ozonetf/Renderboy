#pragma once

#include "Mesh.h"
#include "ShaderFile.hpp"
#include "ShaderProgram.hpp"
#include "texture.h"
#include <filesystem>
#include <main.h>
#include <string>
#include <ufbx.h>
#include <unordered_map>
#include <unordered_set>

class AssetManager
{
  public:
    void     init();
    void     loadTextures();
    texture &getTexture(std::string texName);

    void  loadMeshes();
    Mesh &getMesh(std::string meshName);

    inline static ShaderType getShaderType(std::filesystem::path shaderPath);
    void                     loadShaderFiles();
    GLuint                   getShaderHandle(std::string shaderName, GLuint program);
    void                     updateShaders();

    ShaderProgram *createShaderProgram(std::string name, std::string vertexShaderName, std::string fragShaderName);

  private:
    GLuint                                                 m_missingTexHandle;
    std::unordered_map<std::string, texture>               m_textures;
    std::unordered_map<std::string, Mesh>                  m_meshes;
    std::unordered_map<std::string, ShaderFile>            m_shaderFiles;
    std::unordered_map<std::string, ShaderProgram>         m_shaderPrograms;
    std::unordered_map<GLuint, std::unordered_set<GLuint>> m_shaderRefs;
};

inline ShaderType AssetManager::getShaderType(std::filesystem::path shaderPath)
{
    const auto ext = shaderPath.extension().string();
    if (ext == ".vert")
    {
        return ShaderType::vertex;
    }
    else if (ext == ".frag")
    {
        return ShaderType::fragment;
    }
    else
        return ShaderType::none;
}
