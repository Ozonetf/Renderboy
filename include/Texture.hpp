#pragma once
#include "main.h"
#include <string>

#define STBI_FAILURE_USERMSG
#include <stb_image.h>

// TODO: system to create renderbuffer and framebuffer, set up for depth/stencil
// buffer (should be able to choose either render or frame buffer for depth/stencil)
class Texture
{
  private:
    GLuint m_handle;
    // the image file format (CPU representation)
    GLuint m_pixelFormat = GL_RGB;
    // the format to store as on the GPU
    GLuint m_GPUFormat = GL_RGB;
    int    m_width;
    int    m_height;
    int    m_nChannels;

  public:
    Texture() { glGenTextures(1, &m_handle); };
    ~Texture() = default;

    inline GLuint handle() const { return m_handle; };
    // binds this texture to the current active tex unit
    inline void bindToActiveUnit(GLuint target = GL_TEXTURE_2D) const { glBindTexture(target, m_handle); };
    bool        createFromFile(const char *file, bool createMip);
    bool        createEmpty(const int width, const int height, GLuint format = GL_RGB, GLuint type = GL_UNSIGNED_BYTE);
    void        createDepthStencil(const int width, const int height);
    void        createCubeMap(std::string filename);
    void        createCubeMap();
};
