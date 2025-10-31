#include "perlin_noise.h"

PerlinNoise::PerlinNoise() {
    permutation = {
        151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225,
        140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148,
        247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32,
        57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175,
        74, 165, 71, 134, 139, 48, 27, 166, 102, 143, 54, 63, 18, 102, 225, 231,
        31, 78, 58, 140, 70, 177, 148, 123, 254, 172, 202, 130, 201, 125, 250, 89,
        71, 240, 173, 212, 162, 175, 156, 164, 114, 165, 203, 112, 240, 97, 140, 161,
        137, 13, 113, 52, 37, 114, 42, 44, 175, 226, 18, 91, 36, 37, 252, 47,
        37, 61, 34, 220, 222, 12, 131, 237, 169, 75, 6, 151, 17, 142, 43, 104,
        162, 99, 36, 8, 101, 70, 183, 142, 107, 142, 35, 12, 51, 52, 44, 141,
        111, 98, 31, 98, 19, 137, 36, 78, 55, 60, 2, 65, 32, 117, 84, 69,
        145, 58, 33, 20, 203, 97, 40, 8, 151, 153, 0, 7, 138, 205, 16, 41,
        228, 86, 235, 171, 172, 43, 180, 33, 83, 200, 141, 203, 204, 93, 57, 74,
        76, 88, 207, 208, 239, 170, 251, 67, 77, 51, 133, 69, 249, 2, 127, 80,
        60, 159, 168, 81, 163, 64, 143, 146, 157, 56, 245, 188, 182, 218, 33, 16,
        255, 243, 210, 205, 12, 19, 236, 95, 151, 68, 23, 196, 167, 126, 61, 100,
        93, 25, 115, 96, 129, 79, 220, 34, 42, 144, 136, 70, 238, 184, 20, 222,
        94, 11, 219, 224, 50, 58, 10, 73, 6, 36, 92, 194, 211, 172, 98, 145,
        149, 228, 121, 231, 200, 55, 109, 141, 213, 78, 169, 108, 86, 244, 234, 101,
        122, 174, 8, 186, 120, 37, 46, 28, 166, 180, 198, 232, 221, 116, 31, 75,
        189, 139, 138, 112, 62, 181, 102, 72, 3, 246, 97, 27, 66, 140, 203, 238,
        167, 36, 101, 137, 74, 249, 2, 43, 9, 102, 187, 191, 51, 60, 7, 231,
        200, 43, 65, 51, 13, 161, 136, 63, 65, 132, 84, 69, 66, 107, 34, 232,
        164, 94, 96, 48, 110, 12, 31, 51, 133, 18, 109, 19, 88, 47, 53, 172
    };
    p = permutation;
    p.insert(p.end(), permutation.begin(), permutation.end());
}

PerlinNoise::PerlinNoise(unsigned int seed) : PerlinNoise() {
    for (int i = 0; i < 256; i++) {
        int j = (seed ^ i) % 256;
        std::swap(p[i], p[j]);
        p[i + 256] = p[i];
    }
}

float PerlinNoise::fade(float t) const {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise::lerp(float t, float a, float b) const {
    return a + t * (b - a);
}

float PerlinNoise::grad(int hash, float x, float y, float z) const {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 8 ? y : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise::noise(float x, float y, float z) const {
    int xi = (int)std::floor(x) & 255;
    int yi = (int)std::floor(y) & 255;
    int zi = (int)std::floor(z) & 255;

    float xf = x - std::floor(x);
    float yf = y - std::floor(y);
    float zf = z - std::floor(z);

    float u = fade(xf);
    float v = fade(yf);
    float w = fade(zf);

    int aaa = p[p[p[xi] + yi] + zi];
    int aba = p[p[p[xi] + yi + 1] + zi];
    int aab = p[p[p[xi] + yi] + zi + 1];
    int abb = p[p[p[xi] + yi + 1] + zi + 1];
    int baa = p[p[p[xi + 1] + yi] + zi];
    int bba = p[p[p[xi + 1] + yi + 1] + zi];
    int bab = p[p[p[xi + 1] + yi] + zi + 1];
    int bbb = p[p[p[xi + 1] + yi + 1] + zi + 1];

    float x1 = lerp(u, grad(aaa, xf, yf, zf), grad(baa, xf - 1, yf, zf));
    float x2 = lerp(u, grad(aba, xf, yf - 1, zf), grad(bba, xf - 1, yf - 1, zf));
    float y1 = lerp(v, x1, x2);

    x1 = lerp(u, grad(aab, xf, yf, zf - 1), grad(bab, xf - 1, yf, zf - 1));
    x2 = lerp(u, grad(abb, xf, yf - 1, zf - 1), grad(bbb, xf - 1, yf - 1, zf - 1));
    float y2 = lerp(v, x1, x2);

    return (lerp(w, y1, y2) + 1.0f) / 2.0f;
}

float PerlinNoise::fbm(float x, float y, float z, int octaves, float persistence, float lacunarity) const {
    float result = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; i++) {
        result += noise(x * frequency, y * frequency, z * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    return result / maxValue;
}
