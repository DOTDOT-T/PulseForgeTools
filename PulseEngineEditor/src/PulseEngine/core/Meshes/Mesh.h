#pragma once
#include <glad.h>
#include <vector>
#include "Common/common.h"
#include "dllExport.h"


class MA_DLL_API Mesh
{
public:
    Mesh(const std::vector<float>& vertices);
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    void Draw() const;

private:
    void SetupMesh();

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
};
