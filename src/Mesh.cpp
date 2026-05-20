#include "Mesh.h"
#include "Geometry.h"
#include "Helper.h"

#include <cstddef>
#include <cstdint>
#include <format>
#include <glm/detail/qualifier.hpp>
#include <iostream>
#include <vector>

void Mesh::init()
{
    glGenBuffers(1, &m_EBOhandle);
    glGenBuffers(1, &m_VBOhandle);
    glGenVertexArrays(1, &m_VAOhandle);
}

void Mesh::loadFromFile(const char *fileName, const ufbx_load_opts opts)
{
    // auto opts = ufbx_load_opts{.generate_missing_normals = true};
    auto err = ufbx_error{};
    auto scene = ufbx_load_file(fileName, &opts, &err);
    if (!scene)
    {
        std::cerr << err.description.data;
        return;
    }
    auto mesh = scene->meshes[0];

    std::vector<SimpleVertex> vertices;
    vertices.reserve(mesh->max_face_triangles * mesh->faces.count);
    // the indices of the mesh after triangulation on each faces
    std::vector<uint32_t> triIndices;
    // Safe way to guarantee indicie array size will be no smaller
    // than the actual size. For example the mesh can have n-gons
    // NB need resize here since triIndices.size() is needed late
    triIndices.resize(mesh->max_face_triangles * 3);
    // step trhough each face and triagulate them, for each vertices
    // on a face, add them to the vertex buffer(vertex array)
    for (const auto &face : mesh->faces)
    {
        // triangulate generates all vertices for all triangles
        // in the face, this includes duplecates, which are handeld later
        auto triCount = ufbx_triangulate_face(triIndices.data(), triIndices.size(), mesh, face);
        for (size_t i = 0; i < triCount * 3; ++i)
        {
            uint32_t vertIndex = triIndices[i];
            vertices.push_back({.pos = toGLM(mesh->vertex_position[vertIndex]),
                                .normal = toGLM(mesh->vertex_normal[vertIndex]),
                                .texCoord = toGLM(mesh->vertex_uv[vertIndex])});
        }
    }
    vertices.shrink_to_fit();
    // all verts should be generated per triangle
    assert(vertices.size() == mesh->num_triangles * 3);
    auto vertStream = ufbx_vertex_stream{
        .data = vertices.data(), .vertex_count = vertices.size(), .vertex_size = sizeof(SimpleVertex)};

    std::vector<uint32_t> indices;
    m_iCount = mesh->num_triangles * 3;
    indices.resize(m_iCount);

    m_vCount = ufbx_generate_indices(&vertStream, 1, indices.data(), indices.size(), nullptr, nullptr);
    // trim to only unique
    vertices.resize(m_vCount);
    std::cerr << std::format("loaded {}, vert count: {} ind count: {}\n", mesh->name.data, m_vCount, m_iCount);
    // for (const auto v : vertices)
    // {
    //     printvec3(v.pos);
    // }
    bindBuffer(vertices, indices);
}

void Mesh::render()
{
    glBindVertexArray(m_VAOhandle);
    glDrawElements(GL_TRIANGLES, m_iCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::bindBuffer(const std::vector<SimpleVertex> vertices, const std::vector<uint32_t> indices)
{
    assert(m_VAOhandle != 0 && m_VBOhandle != 0 && m_EBOhandle != 0);
    glBindVertexArray(m_VAOhandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOhandle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SimpleVertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOhandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    setSimpleVertexAttrib();
    // unbind array object when done
    glBindVertexArray(0);
}
