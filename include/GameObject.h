#pragma once
#include "main.h"
#include "shader.h"
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
class GameObject
{
  public:
    GameObject();
    ~GameObject();

    inline glm::mat4 getTransform() const { return m_transform; }
    inline glm::vec3 getPos() const { return m_pos; }

    void updateTransform();
    void rotate(const glm::vec3 _rot);
    void translate(const glm::vec3 _trans);
    void scale(const glm::vec3 _scale);

    void init();
    void render() const;

    void setVertexData(const float *vertexData);
    void setVertexData(const float *vertexData, const int *indiceData);

    shaderProgram m_shader;

  private:
    glm::vec3 m_pos = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 m_rot = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 m_scale = glm::vec3(1.f, 1.f, 1.f);
    glm::mat4 m_transform = glm::mat4();

    GLuint  m_VBOhandle;
    GLuint  m_EBOhandle;
    GLuint  m_VAOhandle;
    GLsizei m_triangleCount = 36;
};
