#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string readFile(const char* filePath);

class Shader {
    public:
        Shader(const char* vertexFp, const char* fragmentFp);
        void setUniform(const char* name, float f);
        void setUniform(const char* name, int i);
        void setUniform(const char* name, glm::mat4 mat4);
        unsigned int shaderProgram;
};

#endif