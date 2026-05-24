#pragma once

#include "Mesh.h"
#include "texture.h"
#include <main.h>
#include <string>
#include <ufbx.h>
#include <unordered_map>

class AssetManager
{
  public:
    void     init();
    void     loadTextures();
    texture &getTexture(std::string texName);

    void  loadMeshes();
    Mesh &getMesh(std::string meshName);

  private:
    GLuint                                   m_missingTexHandle;
    std::unordered_map<std::string, texture> m_textures;
    std::unordered_map<std::string, Mesh>    m_meshes;
};
