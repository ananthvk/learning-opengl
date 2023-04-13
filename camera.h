#pragma once
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gl
{
    class Camera
    {
    public:
        glm::vec3 position;
        glm::vec3 up;
        glm::vec3 target;

        Camera()
        {
            position = glm::vec3(0.0f, 0.0f, 3.0f);
            target = glm::vec3(0.0f, 0.0f, 0.0f);
            // Otherwise do position - target
            // The direction has to be from the targeted point to the camera
            // direction = -glm::normalize(target - position);
            up = glm::vec3(0.0f, 1.0f, 0.0f);
            // Right is a vector which points to the positive x direction
            // of the camera.
            // right = glm::normalize(glm::cross(up, direction));
        }
        glm::mat4 get_lookat()
        {
            return glm::lookAt(position, target, up);
        }
    };
} // namespace  gl
