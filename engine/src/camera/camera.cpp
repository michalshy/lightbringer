#include "camera.h"

constexpr float SPEED = 400.0f;
constexpr float MIN_ZOOM = 0.1f;
constexpr float MAX_ZOOM = 10.0f;
Camera::Camera(float width, float height)
    : m_Zoom(1.0f), m_MovementSpeed(SPEED), // todo: adjust zoom
      m_ViewportWidth(width), m_ViewportHeight(height)
{
    m_ProjectionMatrix = glm::ortho(
        0.0f, m_ViewportWidth,
        0.0f, m_ViewportHeight,
        -1.0f, 1.0f
    );

    m_ViewMatrix = glm::mat4(1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera::SetViewportSize(float width, float height)
{
    m_ViewportWidth = width;
    m_ViewportHeight = height;

    m_ProjectionMatrix = glm::ortho(
        0.0f, m_ViewportWidth,
        0.0f, m_ViewportHeight,
        -1.0f, 1.0f
    );

    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera::SetPosition(glm::vec3 position)
{
    m_Position = position;
    m_Position.x -= m_ViewportWidth/2;
    m_Position.y -= m_ViewportHeight/2;
    m_Zoom = 1.0f; // Reset to base zoom
}

void Camera::RecalculateViewProjectionMatrix()
{
    // View transform: translate by position, then scale by zoom
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(m_Zoom, m_Zoom, 1.0f));

    // View is inverse of world transform
    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
    RecalculateViewProjectionMatrix();
    return m_ViewMatrix;
}

glm::mat4 Camera::GetViewProjectionMatrix()
{
    RecalculateViewProjectionMatrix();
    return m_ViewProjectionMatrix;
}

void Camera::ProcessKeyboard(Camera_Movement direction, float delta_time)
{
    float velocity = m_MovementSpeed * delta_time * m_Zoom;

    if (direction == UP)    m_Position.y += velocity;
    if (direction == DOWN)  m_Position.y -= velocity;
    if (direction == LEFT)  m_Position.x -= velocity;
    if (direction == RIGHT) m_Position.x += velocity;

    RecalculateViewProjectionMatrix();
}


void Camera::ProcessMouseScroll(float yoffset, float mouse_x, float mouse_y)
{
    glm::vec3 before = ScreenToWorld(mouse_x, mouse_y);

    m_Zoom -= yoffset * 0.1f;
    m_Zoom = glm::clamp(m_Zoom, MIN_ZOOM, MAX_ZOOM);

    // After changing zoom we must recompute the view-projection matrix
    // so ScreenToWorld() uses the correct inverse.
    RecalculateViewProjectionMatrix();

    // Compute where the mouse now maps to in world space
    glm::vec3 after = ScreenToWorld(mouse_x, mouse_y);
    // Offset the camera position so the world point under the mouse remains the same
    m_Position += (before - after);

    // Recompute final matrices after adjusting position
    RecalculateViewProjectionMatrix();
}

glm::vec3 Camera::ScreenToWorld(float screenX, float screenY)
{
    float width  = m_ViewportWidth;
    float height = m_ViewportHeight;

    float ndcX =  (screenX / width)  * 2.0f - 1.0f;
    float ndcY = -(screenY / height) * 2.0f + 1.0f; // flip Y for top-left origin

    glm::vec4 ndc = glm::vec4(ndcX, ndcY, 0.0f, 1.0f);

    glm::mat4 invVP = glm::inverse(m_ViewProjectionMatrix);
    glm::vec4 world = invVP * ndc;

    return glm::vec3(world);
}
