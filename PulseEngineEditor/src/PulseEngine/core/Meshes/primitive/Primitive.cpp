#include "Primitive.h"
#include "PulseEngine/core/Meshes/Mesh.h"

Mesh* Primitive::Cube()
{    
    std::vector<float> cubeVertices = 
    {
        // Positions          // Normals
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Bottom-left
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Bottom-right
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Top-right
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // Top-left

        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Bottom-left
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Bottom-right
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Top-right
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // Top-left

        // Left face
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // Bottom-back
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // Bottom-front
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, // Top-front
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, // Top-back

        // Right face
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // Bottom-back
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // Bottom-front
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // Top-front
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // Top-back

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // Back-left
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, // Back-right
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // Front-right
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, // Front-left

        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // Back-left
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // Back-right
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // Front-right
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f  // Front-left
    };

    std::vector<unsigned int> cubeIndices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Bottom face
        16, 17, 18, 18, 19, 16,
        // Top face
        20, 21, 22, 22, 23, 20
    };

    return new Mesh(cubeVertices, cubeIndices);
}