#include "perlin.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <vector>
#include <random>
#include <iostream>

#include <random>
#include <cmath>
#include <glm/glm.hpp>

const static int permutation[] = { 151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

float inv_sq = 1/sqrt(2);

const static std::vector<glm::vec2> gradient_vectors = {
    glm::vec2(1,1),
    glm::vec2(-1,1),
    glm::vec2(1,-1),
    glm::vec2(-1, -1),
    glm::vec2(inv_sq, inv_sq),
    glm::vec2(-inv_sq, inv_sq),
    glm::vec2(-inv_sq, -inv_sq),
    glm::vec2(inv_sq, -inv_sq),
};

glm::vec2 hash(glm::vec2 v, float seed)
{
    // std::srand((v.x * 374761393 + v.y * 668265263) * seed);
    int hash = permutation[permutation[(int)v.x&255]+((int)v.y&255) + ((int)seed & 255)];
    return gradient_vectors[hash % 7];
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
    float amplitude = 1;
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