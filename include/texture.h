#pragma once
#include "main.h"
#include <iostream>

#define STBI_FAILURE_USERMSG
#include <stb_image.h>

class texture
{
  private:
    GLuint m_handle;
    int    m_width;
    int    m_height;
    int    m_nChannels;

  public:
    texture() { glGenTextures(1, &m_handle); };
    ~texture() = default;

    // binds this texture to the current active tex unit
    inline void bindToActiveUnit() const { glBindTexture(GL_TEXTURE_2D, m_handle); };
    bool        createFromFile(const char *file, bool createMip);
};

inline bool texture::createFromFile(const char *file, bool createMip)
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
    else
    {
        int channels;
        if (m_nChannels == 3)
            channels = GL_RGB;
        else if (m_nChannels == 4)
            channels = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, channels, GL_UNSIGNED_BYTE, texSource);
    }
    if (createMip)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // generate mipmap for current active texture unit
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(texSource); // free image from mem after a gl tex has been generated
    return true;
}
