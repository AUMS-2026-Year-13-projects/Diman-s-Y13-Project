#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <string>

std::string readFile(const char* filePath);

class Shader {
    public:
        Shader(const char* vertexFp, const char* fragmentFp);
        unsigned int shaderProgram;
};

#endif