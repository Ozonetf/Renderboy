#include "Mesh.hpp"
#include "Helper.hpp"
#include "Vertex.hpp"
#include "ufbx.h"

#include <cstddef>
#include <cstdint>
#include <format>
#include <glm/detail/qualifier.hpp>
#include <iostream>
#include <vector>

void Mesh::init()
{
    glCreateBuffers(1, &m_EBOhandle);
    glCreateBuffers(1, &m_VBOhandle);
    glCreateVertexArrays(1, &m_VAOhandle);
}

// loads mesh from FBX file, generate normals if missing or otherwised specified
// with ufbx_load_opts, combines all mesh parts into 1 mesh.
// This function bakes the position and normal data of all meshes a fbx contains
// with respect to their original local transform.
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
    logToCerr("mesh count: {}, material count: {}, \n", scene->meshes.count, scene->materials.count);
    if (scene->meshes[0]->generated_normals)
        std::cerr << "Generated normal\n";

    auto mesh = scene->meshes[0];
    if (mesh->vertex_position.exists && mesh->vertex_normal.exists && mesh->vertex_uv.exists)
    {
        makeMesh<VertexType::PosNormalTex>(*scene);
    }
    else
    {
        logToCerr("pos: {} UV: {} Norm: {} color: {}\n", mesh->vertex_position.exists, mesh->vertex_uv.exists,
                  mesh->vertex_normal.exists, mesh->vertex_color.exists);
        assert(false && "vertex type not implemented");
    }
    logToCerr("loaded {}, vert count: {} ind count: {}\n", fileName, m_meshDesc.uniqueVertexCount,
              m_meshDesc.indexCount);
}

void Mesh::render()
{
    glBindVertexArray(m_VAOhandle);
    glDrawElements(GL_TRIANGLES, m_meshDesc.indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

template <VertexType::GLVertex V>
void Mesh::makeMesh(ufbx_scene &scene)
{
    size_t maxModelVertexCount = 0;
    size_t maxModelFaceTri = 0;
    for (const auto &meshPart : scene.meshes)
    {
        maxModelVertexCount += (meshPart->max_face_triangles * meshPart->faces.count);
        maxModelFaceTri = std::max(maxModelFaceTri, meshPart->max_face_triangles);
    }
    std::vector<V> vertices;
    vertices.reserve(maxModelVertexCount);
    // the indices of the mesh after triangulation on each faces
    // Safe way to guarantee indicie array size will be no smaller
    // than the actual size. For example the mesh can have n-gons
    // NB need resize here since triIndices.size() is needed late
    std::vector<uint32_t> triIndices(maxModelFaceTri * 3);

    for (const auto &node : scene.nodes)
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

                V vertex;
                if constexpr (VertexType::HasPos3<V>)
                {
                    auto nodeWorldPos = &node->node_to_world;
                    vertex.pos = toGLM(ufbx_transform_position(nodeWorldPos, mesh->vertex_position[vertIndex]));
                }
                if constexpr (VertexType::HasNormal<V>)
                {
                    // to bake the normal use normalize(inverse(transpose(m)))
                    auto normTransform = ufbx_matrix_for_normals(&node->node_to_world);
                    vertex.normal = toGLM(
                        ufbx_vec3_normalize(ufbx_transform_direction(&normTransform, mesh->vertex_normal[vertIndex])));
                }
                if constexpr (VertexType::HasTexCoord<V>)
                {
                    vertex.texCoord = toGLM(mesh->vertex_uv[vertIndex]);
                }
                vertices.push_back(vertex);
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
    auto vertStream =
        ufbx_vertex_stream{.data = vertices.data(), .vertex_count = vertices.size(), .vertex_size = sizeof(V)};

    auto indices = std::vector<uint32_t>(m_meshDesc.indexCount, 0);

    // trim to only unique
    m_meshDesc.uniqueVertexCount +=
        ufbx_generate_indices(&vertStream, 1, indices.data(), indices.size(), nullptr, nullptr);
    vertices.resize(m_meshDesc.uniqueVertexCount);

    assert(m_VAOhandle != 0 && m_VBOhandle != 0 && m_EBOhandle != 0);
    glNamedBufferStorage(m_VBOhandle, sizeof(V) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(m_EBOhandle, sizeof(uint32_t) * indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayVertexBuffer(m_VAOhandle, 0, m_VBOhandle, 0, sizeof(V));
    glVertexArrayElementBuffer(m_VAOhandle, m_EBOhandle);
    V::setVertexAttribute(m_VAOhandle);
}
