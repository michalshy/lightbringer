#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

struct CoTransform
{
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

    glm::mat4 GetMatrix() const
    {
        glm::mat4 matrix{ 1.0f };

        matrix = glm::translate(matrix, position);

        // Only apply rotation if needed
        matrix = glm::rotate(matrix, rotation.x, glm::vec3(1, 0, 0));
        matrix = glm::rotate(matrix, rotation.y, glm::vec3(0, 1, 0));
        matrix = glm::rotate(matrix, rotation.z, glm::vec3(0, 0, 1));

        matrix = glm::scale(matrix, scale);

        return matrix;
    }

    void Translate(const glm::vec3& offset)
    {
        position += offset;
    }
};

struct CoRigidBody {
    glm::vec2 velocity;
};

struct CoSprite
{
    glm::vec4 tex_rect{ 0.0f, 0.0f, 1.0f, 1.0f };
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

    void UpdateColor(const glm::vec4& vec)
    {
        color = vec;
    }
    void UpdateTex(const glm::vec4& vec)
    {
        tex_rect = vec;
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