#pragma once

#include "Geometry.h"
#include <cstddef>
#include <glad/glad.h>
#include <glm/detail/qualifier.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <ufbx.h>
#include <vector>

class Mesh
{
  public:
    void init();
    void loadFromFile(const char *fileName, const ufbx_load_opts opts = {});
    void render();

    inline GLuint getVBO() const { return m_VBOhandle; };
    inline GLuint getEBO() const { return m_EBOhandle; };
    inline GLuint getVAO() const { return m_VAOhandle; };
    inline size_t getVertCount() const { return m_vCount; };
    inline size_t getIndCount() const { return m_iCount; };

  private:
    void bindBuffer(const std::vector<SimpleVertex> vertices, const std::vector<uint32_t> indices);

    GLuint m_VBOhandle{};
    GLuint m_EBOhandle{};
    GLuint m_VAOhandle{};
    size_t m_vCount = 0;
    size_t m_iCount = 0;
};
