#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
static glm::vec3 UP_DIRECTION = glm::vec3(0.f, 1.f, 0.f);
class Camera
{
  private:
    float     m_fov = 90;
    float     m_near = 0.1f;
    float     m_far = 100.0f;
    glm::vec3 m_pos = glm::vec3(0.f, 0.f, 2.f);
    glm::vec3 m_rot = glm::vec3(0.f, 0.f, 0.f);
    glm::mat4 m_view;
    glm::mat4 m_proj;

  public:
    Camera();
    Camera(float width, float height, float fov, float near, float far);
    ~Camera();

    // Returns view matric of camera with respect to location and rotation
    [[nodiscard]] glm::mat4 getView() const
    {
        return m_view;
    };
    // Returns the projection matrix of the camera based on FOV, aspect ratio and near/far plane.
    [[nodiscard]] glm::mat4 getProj() const
    {
        return m_proj;
    };

    void updateRatio(const float width, const float height)
    {
        m_proj = glm::perspective(m_fov, (width / height), m_near, m_far);
    };

    /// @brief update camera's transform using a translation and rotation with FPS
    /// @param _trans a vec3 vector representing movement in x, y, z with respect to current rotation
    /// @param _pitch rotation in pitch (around the x-axis)
    /// @param _yaw rotation in yaw (around the y-axis)
    void transformCamFPS(glm::vec3 _trans, float _pitch, float _yaw);
};
