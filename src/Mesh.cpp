#include "Mesh.hpp"
#include "Geometry.hpp"
#include "Helper.hpp"
#include "ufbx.h"

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

// loads mesh from FBX file, generate normals if missing or otherwised specified
// with ufbx_load_opts, combines all mesh parts into 1 mesh.
void Mesh::loadModelFromFile(const char *fileName, const ufbx_load_opts opts)
{
    // auto opts = ufbx_load_opts{.generate_missing_normals = true};
    auto err = ufbx_error{};
    auto scene = ufbx_load_file(fileName, &opts, &err);
    if (!scene)
    {
        std::cerr << err.description.data;
        return;
    }
    std::cerr << std::format("mesh count: {}, material count: {}, \n", scene->meshes.count, scene->materials.count);
    if (scene->meshes[0]->generated_normals)
        std::cerr << "Generated normal\n";

    size_t maxModelVertexCount = 0;
    size_t maxModelFaceTri = 0;
    for (const auto &meshPart : scene->meshes)
    {
        maxModelVertexCount += (meshPart->max_face_triangles * meshPart->faces.count);
        maxModelFaceTri = std::max(maxModelFaceTri, meshPart->max_face_triangles);
    }
    std::vector<SimpleVertex> vertices;
    vertices.reserve(maxModelVertexCount);
    // the indices of the mesh after triangulation on each faces
    // Safe way to guarantee indicie array size will be no smaller
    // than the actual size. For example the mesh can have n-gons
    // NB need resize here since triIndices.size() is needed late
    std::vector<uint32_t> triIndices(maxModelFaceTri * 3);

    for (const auto &node : scene->nodes)
    {
        const auto &mesh = node->mesh;
        if (!mesh)
            continue;
        // step trhough each face and triagulate them, for each vertices
        // on a face, add them to the vertex buffer(vertex array)
        size_t generatedVert = 0;
        for (const auto &face : mesh->faces)
        {
            // triangulate generates all vertices for all triangles
            // in the face, this includes duplecates, which are handeld later
            auto triCount = ufbx_triangulate_face(triIndices.data(), triIndices.size(), mesh, face);
            for (size_t i = 0; i < triCount * 3; ++i)
            {
                uint32_t vertIndex = triIndices[i];
                // bake position with respect to nodes world transform
                // TODO: update normal too
                vertices.push_back(
                    {.pos = toGLM(ufbx_transform_position(&node->node_to_world, mesh->vertex_position[vertIndex])),
                     .normal = toGLM(mesh->vertex_normal[vertIndex]),
                     .texCoord = toGLM(mesh->vertex_uv[vertIndex])});
                ++generatedVert;
            }
        }
        size_t meshICount = mesh->num_triangles * 3;
        m_meshDesc.triangleCount += mesh->num_triangles;
        // all verts should be generated per triangle
        assert(generatedVert == mesh->num_triangles * 3);
    }
    vertices.shrink_to_fit();
    m_meshDesc.indexCount = vertices.size();
    auto vertStream = ufbx_vertex_stream{
        .data = vertices.data(), .vertex_count = vertices.size(), .vertex_size = sizeof(SimpleVertex)};

    auto indices = std::vector<uint32_t>(m_meshDesc.indexCount, 0);

    // trim to only unique
    m_meshDesc.uniqueVertexCount +=
        ufbx_generate_indices(&vertStream, 1, indices.data(), indices.size(), nullptr, nullptr);
    vertices.resize(m_meshDesc.uniqueVertexCount);
    std::cerr << std::format("loaded {}, vert count: {} ind count: {}\n", fileName, m_meshDesc.uniqueVertexCount,
                             m_meshDesc.indexCount);
    bindBuffer(vertices, indices);
}

void Mesh::render()
{
    glBindVertexArray(m_VAOhandle);
    glDrawElements(GL_TRIANGLES, m_meshDesc.indexCount, GL_UNSIGNED_INT, 0);
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
