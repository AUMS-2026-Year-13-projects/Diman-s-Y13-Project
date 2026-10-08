#include "camera.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(float f, glm::vec3 pos) {
    direction = glm::vec3(0,0,-1);
    position = pos;
    fov = f;
}