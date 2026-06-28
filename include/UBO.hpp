#pragma once

#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
namespace UBO
{

struct Camera
{
    glm::mat4 proj{};
    glm::mat4 view{};
    glm::vec3 pos{};
};
} // namespace UBO
