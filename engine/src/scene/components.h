#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

struct CoTransform
{
    glm::mat4 transform{1.0};

    void Translate(const glm::vec3& offset)
    {
        transform = glm::translate(transform, offset);
    }
};

struct CoRigidBody {
    glm::vec2 velocity;
};

struct CoSprite
{
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

    void UpdateColor(const glm::vec4& vec)
    {
        color = vec;
    }
};

struct CoCollider {
    glm::ivec2 size;
    bool isStatic = false;
};

struct CoLight
{
    glm::vec3 color = {1.0f, 1.0f, 1.0f};
    float radius = 200.0f;
    float intensity = 1.0f;
    bool active = true;
};