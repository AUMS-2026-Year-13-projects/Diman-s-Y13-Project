#include "perlin.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <vector>
#include <random>
#include <iostream>

#include <random>
#include <cmath>
#include <glm/glm.hpp>

glm::vec2 hash(glm::vec2 v, float seed)
{
    std::srand((v.x * 374761393 + v.y * 668265263) * seed);
    std::vector<glm::vec2> gradient_vectors = {
        glm::vec2(1,1),
        glm::vec2(-1,1),
        glm::vec2(1,-1),
        glm::vec2(-1, -1),
    };

    return gradient_vectors[rand() % 4];
}

float ease_function(float t) {
    return t*t*t*(t*(t*6 - 15) + 10);
}

float lerp(float a, float b, float t) {
    return a + t * (b-a);
}

// float perlina(float x, float y, float cell_size) {
//     glm::vec2 original_coords = glm::vec2(x,y);
//     glm::vec2 normalized_coords = glm::normalize(original_coords);
//     // glm::vec2 normalized_coords = glm::vec2(x / glm::length(original_coords) * cell_size, y / glm::length(original_coords) * cell_size);
//     std::vector<glm::vec2> gradients; 
//     std::vector<glm::vec2> direction;
//     gradients.push_back(hash(glm::vec2(normalized_coords.x, normalized_coords.y)));
//     gradients.push_back(hash(glm::vec2(normalized_coords.x+cell_size, normalized_coords.y)));
//     gradients.push_back(hash(glm::vec2(normalized_coords.x, normalized_coords.y+cell_size)));
//     gradients.push_back(hash(glm::vec2(normalized_coords.x+cell_size, normalized_coords.y+cell_size)));

//     // std::cout << hash(glm::vec2(normalized_coords.x, normalized_coords.y+1)).x << "\n";

//     direction.push_back(normalized_coords);
//     direction.push_back(normalized_coords - glm::vec2(cell_size,0));
//     direction.push_back(normalized_coords - glm::vec2(0,cell_size));
//     direction.push_back(normalized_coords - glm::vec2(cell_size));

//     std::vector<float> dots;
//     for (int i = 0; i<4; i++) {
//         dots.push_back(glm::dot(gradients[0], direction[0]));
//     }

//     float u = ease_function(normalized_coords.x);
//     float v = ease_function(normalized_coords.y);
//     return lerp(lerp(dots[0], dots[1], u), lerp(dots[2], dots[3], v),y);
// };

float perlina(float x, float y, float cell_size, float seed) {
    glm::vec2 cell_coords = glm::vec2(glm::floor(x/cell_size), glm::floor(y/cell_size));
    glm::vec2 local_coords = glm::vec2(x/cell_size - cell_coords.x, y/cell_size - cell_coords.y);

    std::vector<glm::vec2> corners = {
        glm::vec2(cell_coords.x, cell_coords.y),
        glm::vec2(cell_coords.x, cell_coords.y + 1),
        glm::vec2(cell_coords.x + 1, cell_coords.y),
        glm::vec2(cell_coords.x+1, cell_coords.y+1)
    };

    std::vector<float> dots;
    for (int i = 0; i<4; i++) {
        glm::vec2 grad = hash(corners[i], seed);
        glm::vec2 d = glm::vec2(local_coords.x - (corners[i].x - cell_coords.x), local_coords.y - (corners[i].y - cell_coords.y));

        dots.push_back(glm::dot(grad, d));
    }

    float fx = ease_function(local_coords.x);
    float fy = ease_function(local_coords.y);

    float l = dots[0] + fx * (dots[2] - dots[0]);
    float u = dots[1] + fx * (dots[3] - dots[1]);

    float noise = l + fy * (u-l);
    return (noise + 1) / 2;
};

float octavesperlin(float x, float y, float cell_size, float octaves, float persistence, float seed) {
    float total = 0;
    float amplitude = 2;
    float frequency = 1;
    float max_value = 0;
    for (int i = 0; i<octaves; i++) {
        total += perlina(x * frequency, y * frequency, cell_size, seed) * amplitude;

        max_value += amplitude;
        amplitude*=persistence;
        frequency *= 2; 
    }

    return total/max_value;
};