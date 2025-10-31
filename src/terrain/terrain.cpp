#include "terrain.h"
#include <iostream>
#include <cmath>

Terrain::Terrain(int width, int height, float scale, float heightScale)
    : width(width), height(height), scale(scale), heightScale(heightScale),
      noiseGenerator(12345) {
}

void Terrain::generate() {
    mesh.vertices.clear();
    mesh.indices.clear();

    // Generate vertices
    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            float xCoord = (float)x / (width - 1) * scale;
            float zCoord = (float)z / (height - 1) * scale;

            // Use Perlin noise for height
            float noiseVal = noiseGenerator.fbm(xCoord * 0.8f, zCoord * 0.8f, 0.0f, 6, 0.5f, 2.0f);
            float y = noiseVal * heightScale;

            Vector3 pos(xCoord - scale / 2, y, zCoord - scale / 2);
            Vector3 color = getColorByHeight(y);

            Vertex v;
            v.position = pos;
            v.color = color;
            v.normal = Vector3(0, 1, 0); // Will be calculated later

            mesh.vertices.push_back(v);
        }
    }

    generateIndices();
    calculateNormals();
    mesh.setupMesh();

    std::cout << "Terrain generated with " << mesh.vertices.size() << " vertices and "
              << mesh.indices.size() << " indices" << std::endl;
}

void Terrain::generateIndices() {
    for (int z = 0; z < height - 1; z++) {
        for (int x = 0; x < width - 1; x++) {
            int a = z * width + x;
            int b = z * width + (x + 1);
            int c = (z + 1) * width + x;
            int d = (z + 1) * width + (x + 1);

            // First triangle
            mesh.indices.push_back(a);
            mesh.indices.push_back(c);
            mesh.indices.push_back(b);

            // Second triangle
            mesh.indices.push_back(b);
            mesh.indices.push_back(c);
            mesh.indices.push_back(d);
        }
    }
}

void Terrain::calculateNormals() {
    // Initialize normals to zero
    for (auto& v : mesh.vertices) {
        v.normal = Vector3(0, 0, 0);
    }

    // Calculate face normals and accumulate to vertices
    for (size_t i = 0; i < mesh.indices.size(); i += 3) {
        unsigned int i0 = mesh.indices[i];
        unsigned int i1 = mesh.indices[i + 1];
        unsigned int i2 = mesh.indices[i + 2];

        Vector3 v0 = mesh.vertices[i0].position;
        Vector3 v1 = mesh.vertices[i1].position;
        Vector3 v2 = mesh.vertices[i2].position;

        Vector3 edge1 = v1 - v0;
        Vector3 edge2 = v2 - v0;
        Vector3 normal = edge1.cross(edge2).normalized();

        mesh.vertices[i0].normal += normal;
        mesh.vertices[i1].normal += normal;
        mesh.vertices[i2].normal += normal;
    }

    // Normalize the accumulated normals
    for (auto& v : mesh.vertices) {
        v.normal.normalize();
    }
}

Vector3 Terrain::getColorByHeight(float height) {
    // Color gradient based on height
    float normalized = height / heightScale;
    normalized = std::max(0.0f, std::min(1.0f, normalized));

    if (normalized < 0.3f) {
        // Deep: Dark blue to light blue
        float t = normalized / 0.3f;
        return Vector3(0.1f + t * 0.2f, 0.2f + t * 0.3f, 0.4f + t * 0.3f);
    } else if (normalized < 0.45f) {
        // Sand: Light blue to sand
        float t = (normalized - 0.3f) / 0.15f;
        return Vector3(0.3f + t * 0.4f, 0.5f + t * 0.3f, 0.7f - t * 0.3f);
    } else if (normalized < 0.7f) {
        // Grass: Sand to green
        float t = (normalized - 0.45f) / 0.25f;
        return Vector3(0.7f - t * 0.4f, 0.8f + t * 0.15f, 0.4f - t * 0.2f);
    } else if (normalized < 0.85f) {
        // Forest: Green to dark green
        float t = (normalized - 0.7f) / 0.15f;
        return Vector3(0.3f - t * 0.1f, 0.95f - t * 0.3f, 0.2f + t * 0.1f);
    } else {
        // Snow: Dark to white
        float t = (normalized - 0.85f) / 0.15f;
        return Vector3(0.8f + t * 0.2f, 0.8f + t * 0.2f, 0.8f + t * 0.2f);
    }
}
