#pragma once

#include <cstddef>
#include <glad/glad.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace glm;
struct SimpleVertex
{
    vec3 pos;
    vec3 normal;
    vec2 texCoord;
};

// sets the vertex attribute to:
// 0=position
// 2=texCoord
// 3=normal
inline void setSimpleVertexAttrib()
{
    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void *)0);
    glEnableVertexAttribArray(0);
    // texCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void *)offsetof(SimpleVertex, texCoord));
    glEnableVertexAttribArray(2);
    // normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void *)offsetof(SimpleVertex, normal));
    glEnableVertexAttribArray(3);
}
