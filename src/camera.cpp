#include "camera.h"
#include <algorithm>

Camera::Camera() = default;
Camera::~Camera() = default;

Camera::Camera(float width, float height, float fov, float near, float far) : m_near(near), m_far(far), m_fov(fov)
{
    updateRatio(width, height);
}

void Camera::transformCamFPS(glm::vec3 _trans, float _pitch, float _yaw)
{
    m_rot.x += _pitch;
    m_rot.x = std::min(89.f, m_rot.x);
    m_rot.x = std::max(-89.f, m_rot.x);
    m_rot.y -= _yaw;
    const auto quat = glm::quat(glm::radians(m_rot));

    // update camera front
    m_front = glm::normalize(quat * FRONT_DIRECTION);
    // Inverse rotation = conjugate (for unit quaternions)
    const auto rotationMatrix = glm::toMat4(glm::conjugate(quat));

    // rotation then translation
    m_pos += (quat * _trans);
    // inverse translation (-m_pos)
    const auto translationMatrix = glm::translate(glm::mat4(1.0f), -m_pos);

    // usually for matrix is T*R*S, but since camera is inversed, we do R*T
    m_view = rotationMatrix * translationMatrix;
}
