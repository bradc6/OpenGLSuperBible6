#pragma once

//Use the new GLM model behavior
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Camera
{
    public:
        Camera();
        ~Camera();
    protected:
    private:
        glm::mat4 projectionMatrix;
};
