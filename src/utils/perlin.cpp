#include "perlin.hpp"
#include <glm/glm.hpp>
#include <random.hpp>
#include <vector>

uint32_t hash(uint32_t x) {
    x = ((x >> 16) ^ x) * 0x45d9f3bu;
    x = ((x >> 16) ^ x) * 0x45d9f3bu;
    x = (x >> 16) ^ x;
    return x;
}


float perlin(float x, float y) {
    glm::vec2 normalized_coords = glm::normalize(glm::vec2(x,y));
    std::vector<glm::vec2> gradients; 
    std::vector<glm::vec2> direction;
    gradients.push_back(glm::normalize(glm::vec2(hash(normalized_coords.x), hash(normalized_coords.y))));
    gradients.push_back(glm::normalize(glm::vec2(hash(normalized_coords.x+1), hash(normalized_coords.y))));
    gradients.push_back(glm::normalize(glm::vec2(hash(normalized_coords.x), hash(normalized_coords.y+1))));
    gradients.push_back(glm::normalize(glm::vec2(hash(normalized_coords.x+1), hash(normalized_coords.y+1))));

    direction.push_back(normalized_coords);
    direction.push_back(normalized_coords - glm::vec2(1,0));
    direction.push_back(normalized_coords - glm::vec2(0,1));
    direction.push_back(normalized_coords - glm::vec2(1,1));

    std::vector<float> dots;
    for (int i = 0; i<4; i++) {
        dots.push_back(glm::dot(gradients[0], direction[0]));
    }


};