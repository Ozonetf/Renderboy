// A collection of useful helper functions
#pragma once

#include <format>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <random>
#include <ufbx.h>

inline void printvec3(glm::vec3 v) { std::cerr << std::format("x: {} y:{} z:{}\n", v.x, v.y, v.z); }

inline float randomFloat(float min, float max)
{
    static std::random_device             rd;
    static std::mt19937                   gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

inline glm::vec3 toGLM(ufbx_vec3 _v)
{
    return glm::vec3(static_cast<float>(_v.x), static_cast<float>(_v.y), static_cast<float>(_v.z));
}

inline glm::vec2 toGLM(ufbx_vec2 _v) { return glm::vec2(static_cast<float>(_v.x), static_cast<float>(_v.y)); }
