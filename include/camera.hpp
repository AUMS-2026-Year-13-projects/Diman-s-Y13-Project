#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
    public:
        Camera(float fov, glm::vec3 pos);
        float fov;
        float yaw;
        float pitch;
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 right;
};

#endif