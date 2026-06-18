#pragma once

#include "Geometry.hpp"
#include "Material.hpp"
#include <cstddef>
#include <glad/glad.h>
#include <glm/detail/qualifier.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <ufbx.h>
#include <vector>

struct MeshDesc
{
    size_t indexCount{};
    size_t triangleCount{};
    size_t uniqueVertexCount{};
};

class Mesh
{
  public:
    void init();
    void loadModelFromFile(const char          *fileName,
                           const ufbx_load_opts opts = {.generate_missing_normals = true,
                                                        .space_conversion = UFBX_SPACE_CONVERSION_ADJUST_TRANSFORMS,
                                                        .target_unit_meters = 1.0f});
    void render();

    inline GLuint   getVBO() const { return m_VBOhandle; };
    inline GLuint   getEBO() const { return m_EBOhandle; };
    inline GLuint   getVAO() const { return m_VAOhandle; };
    inline MeshDesc getDesc() const { return m_meshDesc; };

  private:
    void bindBuffer(const std::vector<SimpleVertex> vertices, const std::vector<uint32_t> indices);

    GLuint   m_VBOhandle{};
    GLuint   m_EBOhandle{};
    GLuint   m_VAOhandle{};
    MeshDesc m_meshDesc{};
    Material m_mat{};
};
