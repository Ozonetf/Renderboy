#pragma once
#include "main.h"
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
class GameObject
{
  public:
    GameObject() = default;
    GameObject(glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale) : m_pos(_pos), m_rot(_rot), m_scale(_scale) {};
    ~GameObject() = default;

    glm::vec3 m_pos = glm::vec3(0.f, 0.f, 0.f);

    inline void rotate(const glm::vec3 _rot) { m_rot += _rot; };
    inline void translate(const glm::vec3 _trans) { m_pos += _trans; };
    inline void scale(const glm::vec3 _scale) { m_scale += _scale; };
    // inline glm::vec3 getPos() const { return m_pos; }
    inline glm::mat3 getNormalTransform() { return m_normalMatrix; };
    inline glm::mat4 getTransform() const { return m_transform; }

    void updateTransform();
    void init();
    void render() const;

    void setVertexData(const float *vertexData);
    void setVertexData(const float *vertexData, const int *indiceData);

  private:
    glm::vec3 m_rot = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 m_scale = glm::vec3(1.f, 1.f, 1.f);

    // if the mesh's normal is read from vertex attributes and
    // not calculated using the current polygon, special transform
    // needs to be applied to the normal to account for the object's
    // rotation and scale
    glm::mat3 m_normalMatrix = glm::mat3();
    glm::mat4 m_transform = glm::mat4();

    GLuint  m_VBOhandle;
    GLuint  m_EBOhandle;
    GLuint  m_VAOhandle;
    GLsizei m_vertCount = 36;
};
