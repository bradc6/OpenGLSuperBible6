#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
Camera::Camera()
{
    m_Position = glm::vec3(1.0f, 0.0f, 0.0f);
    m_Front = glm::vec3(0.0f, 1.0f, 0.0f);
    m_Up = glm::vec3(0.0f, 0.0f, 1.0f);
    m_Speed = 0.05f;
}

glm::mat4 Camera::GetCameraMatrix()
{
    return glm::lookAt(m_Position,
                       m_Front,
                       m_Up);
}
