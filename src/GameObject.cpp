#include "GameObject.h"
#include "main.h"
#include <glm/gtx/quaternion.hpp>

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}
void GameObject::updateTransform()
{
    auto scaleMat = glm::scale(glm::mat4(1.0f), m_scale);
    auto quat = glm::quat(glm::radians(m_rot));
    auto rotMat = glm::toMat4(quat);
    auto transMat = glm::translate(glm::mat4(1.0f), m_pos);
    // S*R*T, matrix mult is reversed
    m_transform = transMat * rotMat * scaleMat;
}

void GameObject::rotate(const glm::vec3 _rot)
{
    m_rot += _rot;
}
void GameObject::translate(const glm::vec3 _trans)
{
    m_pos += _trans;
}
void GameObject::scale(const glm::vec3 _scale)
{
    m_scale += _scale;
}
void GameObject::init()
{
    glGenBuffers(1, &m_EBOhandle);
    glGenBuffers(1, &m_VBOhandle);
    glGenVertexArrays(1, &m_VAOhandle);
}

// TODO: correctly pass in the size of buffer data with respect to the
// vertex attrubute, e.g. VA_PT = vertCount * 5
void GameObject::setVertexData(const float *vertexData)
{
    assert(vertexData != nullptr);
    glBindVertexArray(m_VAOhandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOhandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 5, vertexData, GL_STATIC_DRAW);
    setVA_PT();
    // unbind array object when done
    glBindVertexArray(0);
}

void GameObject::setVertexData(const float *vertexData, const int *indiceData)
{
    assert(vertexData != nullptr && indiceData != nullptr);
    glBindVertexArray(m_VAOhandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOhandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOhandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indiceData), indiceData, GL_STATIC_DRAW);

    // unbind array object when done
    glBindVertexArray(0);
    return;
}

// TODO: set up proper rendering using mesh and associated VAO, trianglecount etc
void GameObject::render()
{
    // std::cerr << m_VAOhandle << '\n';
    glBindVertexArray(m_VAOhandle);
    glDrawArrays(GL_TRIANGLES, 0, m_triangleCount);
    // glBindVertexArray(0);
}
