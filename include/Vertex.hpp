#pragma once

#include <concepts>
#include <cstddef>
#include <glad/glad.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace glm;
enum class Vertex_Attribute : GLuint
{
    POSITION = 0,
    COLOR = 1,
    UV = 2,
    NORMAL = 3,
    POSITION2D = 4,
};
using loc_of = std::underlying_type_t<Vertex_Attribute>;

inline void vertexAttributeFormat(GLuint VAO, Vertex_Attribute atrb, GLuint offset, GLuint size, GLuint type = GL_FLOAT,
                                  GLuint normalize = GL_FALSE)
{
    glEnableVertexArrayAttrib(VAO, loc_of(atrb));
    glVertexArrayAttribFormat(VAO, loc_of(atrb), size, type, normalize, offset);
    glVertexArrayAttribBinding(VAO, loc_of(atrb), 0);
}

namespace VertexType
{
// requires the type to have setVertexAttribute function
template <typename V>
concept GLVertex = requires(GLuint VAO) { V::setVertexAttribute(VAO); };

template <typename V>
concept HasNormal = GLVertex<V> && requires(V v) {
    { v.normal } -> std::convertible_to<glm::vec3>;
};

template <typename V>
concept HasPos3 = GLVertex<V> && requires(V v) {
    { v.pos } -> std::convertible_to<glm::vec3>;
};

template <typename V>
concept HasPos2 = GLVertex<V> && requires(V v) {
    { v.pos } -> std::convertible_to<glm::vec2>;
};

template <typename V>
concept HasTexCoord = GLVertex<V> && requires(V v) {
    { v.texCoord } -> std::convertible_to<glm::vec2>;
};

template <typename V>
concept HasColour = GLVertex<V> && requires(V v) {
    { v.color } -> std::convertible_to<glm::vec3>;
};

struct PosNormalTex
{
    vec3 pos;
    vec3 normal;
    vec2 texCoord;

    static void setVertexAttribute(GLuint VAO)
    {
        vertexAttributeFormat(VAO, Vertex_Attribute::POSITION, offsetof(PosNormalTex, pos), 3);
        vertexAttributeFormat(VAO, Vertex_Attribute::NORMAL, offsetof(PosNormalTex, normal), 3);
        vertexAttributeFormat(VAO, Vertex_Attribute::UV, offsetof(PosNormalTex, texCoord), 2);
    }
};

struct Pos
{
    vec3 pos;

    static void setVertexAttribute(GLuint VAO)
    {
        vertexAttributeFormat(VAO, Vertex_Attribute::POSITION, offsetof(Pos, pos), 3);
    }
};
struct Pos2dTex
{
    vec2 pos;
    vec2 texCoord;

    static void setVertexAttribute(GLuint VAO)
    {
        vertexAttributeFormat(VAO, Vertex_Attribute::POSITION2D, offsetof(Pos2dTex, pos), 2);
        vertexAttributeFormat(VAO, Vertex_Attribute::UV, offsetof(Pos2dTex, texCoord), 2);
    }
};

}; // namespace VertexType
