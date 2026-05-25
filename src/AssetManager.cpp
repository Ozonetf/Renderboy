#include "AssetManager.hpp"
#include "Mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include <GL/gl.h>
#include <filesystem>
#include <format>
#include <stb_image.h>
#include <ufbx.h>

void AssetManager::init()
{
    // OpenGL tex orientation style
    stbi_set_flip_vertically_on_load(true);
    // init missing texture
    {
        auto t = m_textures.try_emplace("missingTexture");
        t.first->second.createFromFile("assets/textures/missingtex.png", true);

        assert(t.second);
    }
}

void AssetManager::loadTextures()
{
    auto textureFolder = std::filesystem::path{"assets/textures"};
    for (const auto &texFile : std::filesystem::directory_iterator(textureFolder))
    {
        auto filename = texFile.path().filename();
        std::cerr << std::format("loading {}\n", filename.string());
        auto iter = m_textures.try_emplace(filename.stem().string());
        // Create a generic string and conver to char, otherwise on windows c_str returns
        // w_char. need to create a copy so creatFromFile has a valid char pointer.
        auto pathString = texFile.path().string();
        auto succ = iter.first->second.createFromFile(pathString.c_str(), true);
        // TODO:: better impl? currently creating a tex no matter if file is valid/exsist or not
        if (!succ)
            m_textures.erase(iter.first);
    }
}

texture &AssetManager::getTexture(std::string texName)
{
    auto tex = m_textures.find(texName);
    if (tex != m_textures.end())
    {
        return tex->second;
    }
    else
    {
        std::cerr << std::format("AssetManager Error: {} not found\n", texName);
        return m_textures.at("missingTexture");
    }
}

void AssetManager::loadMeshes()
{
    auto meshFolder = std::filesystem::path{"assets/mesh"};
    for (const auto &fbxFile : std::filesystem::directory_iterator(meshFolder))
    {
        std::cerr << std::format("loading {}\n", fbxFile.path().string());
        auto mesh = &m_meshes.try_emplace(fbxFile.path().filename().stem().string(), Mesh{}).first->second;
        mesh->init();
        auto pathString = fbxFile.path().string();
        mesh->loadModelFromFile(pathString.c_str());
    }
}

Mesh &AssetManager::getMesh(std::string meshName)
{
    auto iter = m_meshes.find(meshName);
    if (iter == m_meshes.end())
        std::cerr << meshName << " mesh not found\n";
    return m_meshes.at(meshName);
}
