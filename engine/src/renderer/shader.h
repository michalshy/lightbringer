#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <string>
#include "engine_api.h"
class Shader
{
    int ID;
public:
    ENGINE_API Shader();
    ENGINE_API Shader(const char* vertexPath, const char* fragmentPath);
    ENGINE_API void Use();
    ENGINE_API void SetBool(const std::string& name, bool value) const;
    ENGINE_API void SetInt(const std::string& name, int value) const;
    ENGINE_API void SetFloat(const std::string& name, float value) const;
    ENGINE_API void SetFloat4(const std::string& name, float x, float y, float z, float w) const;
    ENGINE_API void SetMat4(const std::string& name, const glm::mat4& m);
    ENGINE_API void SetVec2(const std::string& name, const glm::vec2& vec) const;
    ENGINE_API void SetVec3(const std::string& name, float x, float y, float z) const;
    ENGINE_API void SetVec3(const std::string& name, glm::vec3& vec) const;
    ENGINE_API void SetVec3(const std::string& name, const glm::vec3& vec) const;
    ENGINE_API void SetIntArray(const char* name, int* values, int count) const;
    ENGINE_API int GetId();
};