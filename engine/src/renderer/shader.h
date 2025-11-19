#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <string>
class Shader
{
    int ID;
public:
    Shader();
    Shader(const char* vertexPath, const char* fragmentPath);
    void Use();
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetFloat4(const std::string& name, float x, float y, float z, float w) const;
    void SetMat4(const std::string& name, const glm::mat4& m);
    void SetVec2(const std::string& name, const glm::vec2& vec) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;
    void SetVec3(const std::string& name, glm::vec3& vec) const;
    void SetVec3(const std::string& name, const glm::vec3& vec) const;
    void SetIntArray(const char* name, int* values, int count) const;
    int GetId();
};