#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

class PerlinNoise {
public:
    // Fade function
    static float fade(float t);

    // Linear interpolation function
    static float lerp(float a, float b, float t);
    
    // Gradient function
    static float grad(int hash, float x, float y, float z);
    
    // Fractal Brownian Motion method
    static float fbm(float x, float y, float z, int octaves);
};

#endif // PERLIN_NOISE_H
