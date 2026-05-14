#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>

static glm::vec3 UP_DIRECTION = glm::vec3(0.f, 1.f, 0.f);
static glm::vec3 FRONT_DIRECTION = glm::vec3(0, 0, -1.f); // OpenGL is righ hand, -Z is front
class Camera
{
  private:
    float     m_fov = 90;
    float     m_near = 0.1f;
    float     m_far = 100.0f;
    glm::vec3 m_pos = glm::vec3(0.f, 0.f, 2.f);
    glm::vec3 m_rot = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 m_front = FRONT_DIRECTION;
    glm::mat4 m_view = glm::mat4(1.0f);
    glm::mat4 m_proj = glm::mat4(1.0f);

  public:
    Camera();
    Camera(float width, float height, float fov, float near, float far);
    ~Camera();

    inline glm::vec3 getPos() const { return m_pos; };
    inline glm::vec3 getDir() const { return m_rot; };
    inline glm::vec3 getFront() const { return m_front; };
    // Returns look matrix of camera with respect to the position and rotation
    [[nodiscard]] inline glm::mat4 getView() const { return m_view; };
    // Returns the projection matrix of the camera based on FOV, aspect ratio and near/far plane.
    [[nodiscard]] inline glm::mat4 getProj() const { return m_proj; };

    void updateRatio(const float width, const float height)
    {
        m_proj = glm::perspective(glm::radians(m_fov), (width / height), m_near, m_far);
    };

    /// @brief update camera's transform using a translation and rotation with FPS
    /// @param _trans a vec3 vector representing movement in x, y, z with respect to current rotation
    /// @param _pitch rotation in pitch (around the x-axis)
    /// @param _yaw rotation in yaw (around the y-axis)
    void transformCamFPS(glm::vec3 _trans, float _pitch, float _yaw);
};
