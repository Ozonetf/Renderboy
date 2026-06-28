#include "Texture.hpp"
#include "Helper.hpp"
#include "main.h"
#include <filesystem>
#include <iostream>

bool Texture::createFromFile(const char *file, bool createMip)
{
    auto texSource = stbi_load(file, &m_width, &m_height, &m_nChannels, 0);
    bindToActiveUnit();
    // missing texture
    if (!texSource)
    {
        std::cerr << "error loading texture: " << file << '\n';
        std::cerr << stbi_failure_reason() << '\n';
        return false;
    }
    switch (m_nChannels)
    {
    case 1:
        m_pixelFormat = GL_RED;
        break;
    case 2:
        m_pixelFormat = GL_RG;
        break;
    case 3:
        m_pixelFormat = GL_RGB;
        break;
    case 4:
        m_pixelFormat = GL_RGBA;
        break;
    default:
        logToCerr("ERROR: {} has invalid channel count", file);
        return false;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, m_GPUFormat, m_width, m_height, 0, m_pixelFormat, GL_UNSIGNED_BYTE, texSource);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    if (createMip) // generate mipmap for current active texture unit
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(texSource); // free image from mem after a gl tex has been generated
    return true;
}

bool Texture::createEmpty(const int width, const int height, GLuint format, GLuint type)
{
    bindToActiveUnit();
    // pass in NULL to allocate but not initilize data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    return 0;
}

void Texture::createDepthStencil(const int width, const int height)
{
    // DSA
    // glCreateTextures(GL_DEPTH_STENCIL, 1, &this->m_handle);
    bindToActiveUnit();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_BYTE, NULL);
    // bind with:
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, tbo.handle(), 0);
}

void Texture::createCubeMap(std::string filename)
{
    auto skyboxFolder = std::filesystem::path{"assets/textures/skybox/"};
    for (const auto &st : std::filesystem::directory_iterator(skyboxFolder))
    {
        logToCerr("{}", st.path().filename().string());
    }
    // NB: not sure why skybox texture dont need to be fliped
    stbi_set_flip_vertically_on_load(false);
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &this->m_handle);
    int      w, h, c;
    stbi_uc *faces[6];
    // load all textures from file, get dimenstion info
    constexpr const char *skyboxFileName = "assets/textures/skybox/";
    for (size_t i = 0; i < 6; ++i)
    {
        auto faceFile = std::string{skyboxFileName} + filename + std::to_string(i) + ".jpg";
        logToCerr("loading {}\n", faceFile);
        faces[i] = stbi_load(faceFile.c_str(), &m_width, &m_height, &c, 0);
        if (!faces[i])
        {
            logToCerr("error loading texture: {} {}\n", faceFile, stbi_failure_reason());
            // return false;
        }
    }
    // zoffset Cubemap face
    // 0 = GL_TEXTURE_CUBE_MAP_POSITIVE_X = right
    // 1 = GL_TEXTURE_CUBE_MAP_NEGATIVE_X = left
    // 2 = GL_TEXTURE_CUBE_MAP_POSITIVE_Y = top
    // 3 = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y = bottom
    // 4 = GL_TEXTURE_CUBE_MAP_POSITIVE_Z = front
    // 5 = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z = back
    glTextureStorage2D(this->m_handle, 1, GL_RGB8, m_width, m_height);
    const int miplvl = 0;
    for (size_t i = 0; i < 6; ++i)
    {
        glTextureSubImage3D(this->m_handle, miplvl, 0, 0, i, m_width, m_height, 1, m_pixelFormat, GL_UNSIGNED_BYTE,
                            faces[i]);
        stbi_image_free(faces[i]);
    }

    glTextureParameteri(this->m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(this->m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(this->m_handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(this->m_handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(this->m_handle, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    stbi_set_flip_vertically_on_load(true);
}

// Automatically immports skybox texture in the skybox texture folder, the textures
// need to have the name: px, nx, py, ny, pz, nz
void Texture::createCubeMap()
{
    stbi_set_flip_vertically_on_load(false);
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &this->m_handle);
    int      c;
    stbi_uc *faces[6];
    auto     skyboxFolder = std::filesystem::path{"assets/textures/skybox/"};
    for (const auto &st : std::filesystem::directory_iterator(skyboxFolder))
    {
        auto stn = st.path().string();
        auto n = st.path().filename().stem().string();
        if (n == "px")
            faces[0] = stbi_load(stn.c_str(), &m_width, &m_height, &c, 0);
        else if (n == "nx")
            faces[1] = stbi_load(stn.c_str(), &m_width, &m_height, &c, 0);
        else if (n == "py")
            faces[2] = stbi_load(stn.c_str(), &m_width, &m_height, &c, 0);
        else if (n == "ny")
            faces[3] = stbi_load(stn.c_str(), &m_width, &m_height, &c, 0);
        else if (n == "pz")
            faces[4] = stbi_load(stn.c_str(), &m_width, &m_height, &c, 0);
        else if (n == "nz")
            faces[5] = stbi_load(stn.c_str(), &m_width, &m_height, &c, 0);
    }
    if (c == 4)
        m_pixelFormat = GL_RGBA;
    GLuint GPUformat = c == 3 ? GL_RGB8 : GL_RGBA8;
    for (size_t i = 0; i < 6; ++i)
    {
        if (!faces[i])
        {
            logToCerr("error loading skybox texture: 0:px, 1:nx, 2:py, 3:ny, 4:pz, 5:nz :: {}\n", i);
            // return false;
        }
    }
    glTextureStorage2D(this->m_handle, 1, GPUformat, m_width, m_height);
    const int miplvl = 0;
    for (size_t i = 0; i < 6; ++i)
    {
        glTextureSubImage3D(this->m_handle, miplvl, 0, 0, i, m_width, m_height, 1, m_pixelFormat, GL_UNSIGNED_BYTE,
                            faces[i]);
        stbi_image_free(faces[i]);
    }

    glTextureParameteri(this->m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(this->m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(this->m_handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(this->m_handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(this->m_handle, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    stbi_set_flip_vertically_on_load(true);
}
