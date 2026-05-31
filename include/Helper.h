// A collection of useful helper functions
#pragma once

#include <filesystem>
#include <format>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <random>
#include <ufbx.h>

inline float randomFloat(float min, float max)
{
    static std::random_device             rd;
    static std::mt19937                   gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

// wraper to write to cerr using std::format strings
template <class... Args>
inline void logToCerr(std::format_string<Args...> fmt, Args &&...args)
{
    std::format_to(std::ostreambuf_iterator<char>(std::cerr), fmt, std::forward<Args>(args)...);
}

inline void printvec3(glm::vec3 v) { logToCerr("x: {} y:{} z:{}\n", v.x, v.y, v.z); }

inline glm::vec3 randVec3(float min, float max)
{
    return glm::vec3(randomFloat(min, max), randomFloat(min, max), randomFloat(min, max));
}

inline glm::vec3 toGLM(ufbx_vec3 _v)
{
    return glm::vec3(static_cast<float>(_v.x), static_cast<float>(_v.y), static_cast<float>(_v.z));
}

inline glm::vec2 toGLM(ufbx_vec2 _v) { return glm::vec2(static_cast<float>(_v.x), static_cast<float>(_v.y)); }

inline std::string fileTimeToString(const std::filesystem::file_time_type &ftime)
{
    std::time_t cftime = std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(ftime));
    std::string str = std::asctime(std::localtime(&cftime));
    str.pop_back(); // rm the trailing '\n' put by `asctime`
    return str;
}
