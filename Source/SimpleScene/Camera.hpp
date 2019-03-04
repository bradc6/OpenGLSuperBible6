#pragma once

//Use the new GLM model behavior
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Camera
{
    public:
        Camera();

        glm::mat4 GetCameraMatrix();

        float m_Speed;
        glm::vec3 m_Position;
        glm::vec3 m_Front;
        glm::vec3 m_Up;
};
