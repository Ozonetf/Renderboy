#pragma once

#include "Material.hpp"
#include "Vertex.hpp"
#include <cstddef>
#include <glad/glad.h>
#include <glm/detail/qualifier.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <ufbx.h>

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

    template <VertexType::GLVertex V>
    void makeMesh(const void *data, size_t size);

    inline MeshDesc getDesc() const { return m_meshDesc; };

  private:
    template <VertexType::GLVertex V>
    void makeMesh(ufbx_scene &scene);

    GLuint   m_VBOhandle{};
    GLuint   m_EBOhandle{};
    GLuint   m_VAOhandle{};
    MeshDesc m_meshDesc{};
    Material m_mat{};
};

// Mesh vertex buffer setup using specified GLVertex, useful
// for setting up small vertex buffer arrays. Note: GLdrawarray
// is called rather than GLdraw ellement since this doesnt set up
// the index buffer
// @params
// @data: pointer to vertex buffer
// @size: size of vertex data in bytes
template <VertexType::GLVertex V>
void Mesh::makeMesh(const void *data, size_t size)
{
    glDeleteBuffers(1, &m_EBOhandle);
    m_EBOhandle = 0;
    assert(size % sizeof(V) == 0 &&
           "Vertex buffer size doesn't align with vertex type, did you specified the right type?");

    int dataVertCount = size / sizeof(V);
    m_meshDesc.indexCount = m_meshDesc.uniqueVertexCount = dataVertCount;
    m_meshDesc.triangleCount = dataVertCount / 3;

    glNamedBufferStorage(m_VBOhandle, size, data, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayVertexBuffer(m_VAOhandle, 0, m_VBOhandle, 0, sizeof(V));
    V::setVertexAttribute(m_VAOhandle);
}
