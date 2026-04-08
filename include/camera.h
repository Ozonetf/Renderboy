#pragma once

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
static glm::vec3 UP_DIRECTION = glm::vec3(0.f, 1.f, 0.f);
class Camera
{
private:
    // inline glm::vec3 getCamDir() const {return glm::normalize(m_pos - cameraTarget);};
    inline glm::vec3 getRight() const {return glm::normalize(glm::cross(m_dir, UP_DIRECTION));};
    inline glm::vec3 getUp() const {return glm::normalize(glm::cross(m_dir, m_right));};

    // glm::vec3 cameraTarget  = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_pos         = glm::vec3(0.f, 0.f, 2.f);
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_dir;
    glm::mat4 m_lookat;
public:
    inline Camera(/* args */);
    inline ~Camera();

    inline glm::mat4 getLookAt() const {return m_lookat;};

    /// @brief update camere's transform using a translation and rotation with FPS
    /// @param _trans a vec3 vector representing movement in x, y, z with respect to current rotation
    /// @param _pitch rotation in pitch (around the x axis)
    /// @param _yaw rotation in yaw (around the y axis)
    inline void transformCamFPS(glm::vec3 _trans, float _pitch, float _yaw);
    inline void update();
};
Camera::Camera(/* args */)
{
    m_pos = glm::vec3(0.0f, 0.0f, 2.0f);
    m_dir = glm::vec3(0.0f, 0.0f, -1.0f);
    m_right = getRight();
    m_up = getUp();
}

Camera::~Camera()
{
}

void Camera::transformCamFPS(glm::vec3 _trans, float _pitch, float _yaw)
{
    m_pos += _trans;
    m_lookat = glm::lookAt(m_pos, m_pos + m_dir, m_up);
}

// inline void camera::update()
// {
//     const float radius = 2.0f;
//     m_pos.x = sin(glfwGetTime()) * radius;
//     m_pos.z = cos(glfwGetTime()) * radius;
//     m_dir = getCamDir();
//     m_right = getRight();
//     m_up = getUp();
//     m_lookat = glm::lookAt(m_pos, cameraTarget, m_up);
// }