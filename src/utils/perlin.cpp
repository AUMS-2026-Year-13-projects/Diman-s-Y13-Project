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
    float amplitude = 3;
    float frequency = 1;
    float max_value = 0;
    for (int i = 0; i<octaves; i++) {
        total += perlina(x * frequency, y * frequency, cell_size, seed) * amplitude;
        max_value += amplitude;
        amplitude*=persistence;
        frequency *= 0.6; 
    }

    return total/max_value;
};