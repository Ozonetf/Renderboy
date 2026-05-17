#include "GameObject.h"

void GameObject::updateTransform()
{
    auto scaleMat = glm::scale(glm::mat4(1.0f), m_scale);
    auto quat = glm::quat(glm::radians(m_rot));
    auto rotMat = glm::mat4(quat);
    auto transMat = glm::translate(glm::mat4(1.0f), m_pos);
    // S*R*T, matrix mult is reversed
    m_transform = transMat * rotMat * scaleMat;
    m_normalMatrix = glm::mat3(glm::transpose(glm::inverse(m_transform)));
}

void GameObject::init() { updateTransform(); }

// TODO: correctly pass in the size of buffer data with respect to the
// vertex attrubute, e.g. VA_PT = vertCount * 5

// TODO: set up proper rendering using mesh and associated VAO, trianglecount etc
void GameObject::render() const { m_mesh->render(); }
