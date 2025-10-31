#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "../graphics/mesh.h"
#include "perlin_noise.h"
#include "../math/math.h"

class Terrain {
public:
    Mesh mesh;
    int width, height;
    float scale;
    float heightScale;

    Terrain(int width = 200, int height = 200, float scale = 1.0f, float heightScale = 50.0f);

    void generate();
    void generateWithHeightmap(float minHeight, float maxHeight);
    void calculateNormals();
    Vector3 getColorByHeight(float height);
    void draw() const { mesh.draw(); }

private:
    PerlinNoise noiseGenerator;
    void generateIndices();
};

#endif // TERRAIN_H
