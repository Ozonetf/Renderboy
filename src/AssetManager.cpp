#include "AssetManager.hpp"
#include "Helper.h"

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
        if (texFile.is_directory())
            continue;
        auto filename = texFile.path().filename();
        logToCerr("loading {}\n", filename.string());
        auto [iter, b] = m_textures.try_emplace(filename.stem().string());
        auto &[tname, tex] = *iter;
        // Create a generic string and conver to char, otherwise on windows c_str returns
        // w_char. need to create a copy so creatFromFile has a valid char pointer.
        auto pathString = texFile.path().string();
        auto succ = tex.createFromFile(pathString.c_str(), true);
        // TODO:: better impl? currently creating a tex no matter if file is valid/exsist or not
        if (!succ)
            m_textures.erase(iter);
    }
}

Texture &AssetManager::getTexture(std::string texName)
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
        auto lswt = std::filesystem::last_write_time(fbxFile);
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

void AssetManager::loadShaderFiles()
{
    const auto shaderFolder = std::filesystem::path{"assets/shaders"};
    for (const auto &dirEntry : std::filesystem::directory_iterator(shaderFolder))
    {
        auto shaderFileName = dirEntry.path().filename();
        auto fileType = getShaderType(dirEntry.path());
        if (fileType == none)
            continue;
        auto [ele, b] = m_shaderFiles.try_emplace(shaderFileName.string(), dirEntry);
        logToCerr("loading {}\n", shaderFileName.string());
        ele->second.init(dirEntry, fileType);
    }
}

// Request a shader handle for a program, caches the program using the shader
GLuint AssetManager::getShaderHandle(std::string shaderName, GLuint program)
{
    auto iter = m_shaderFiles.find(shaderName);
    if (iter == m_shaderFiles.end())
    {
        return 0;
    }
    else
    {
        auto &shader = iter->second;
        auto  handle = shader.requestHandle();
        // init shader ref list lazily, does nothing if it alread exsists
        m_shaderRefs.try_emplace(handle);
        m_shaderRefs.at(handle).insert(program);
        return handle;
    }
}

void AssetManager::updateShaders()
{
    static auto programReloadList = std::unordered_set<GLuint>{};
    const auto  shaderFolder = std::filesystem::path{"assets/shaders"};
    for (auto &iter : m_shaderFiles)
    {
        auto shaderFile = iter.second;
        auto newTime = std::filesystem::last_write_time(shaderFile.getdir());
        auto oldTime = shaderFile.lastWrite();
        // If shader file modified, recompile the object, then add all referencing program to list to relink
        if (oldTime != newTime)
        {
            logToCerr("{} Modified, reloading\n", shaderFile.m_dirEntry.path().filename().string());
            auto  shaderHandle = shaderFile.reload(newTime);
            auto &programRefs = m_shaderRefs.at(shaderHandle);
            programReloadList.insert(programRefs.begin(), programRefs.end());
        }
    }
    for (auto program : programReloadList)
    {
        logToCerr("updating program {}\n", program);
        ShaderProgram::reload(program);
    }
    programReloadList.clear();
}
ShaderProgram *AssetManager::createShaderProgram(std::string name, std::string vertexShaderName,
                                                 std::string fragShaderName)
{
    auto [iter, b] = m_shaderPrograms.try_emplace(name);
    auto &[n, sp] = *iter;
    auto vertHandle = getShaderHandle(vertexShaderName, sp.m_handle);
    auto fragHandle = getShaderHandle(fragShaderName, sp.m_handle);
    sp.attach(vertHandle);
    sp.attach(fragHandle);
    sp.compile();
    logToCerr("Shader program {} \tShader Handle {}\n", name, sp.m_handle);
    return &sp;
}
