#include "Material.h"
#include "shader.h"

Shader *Material::GetShader()
{
    if(!shader) shader = new Shader("shaders/basic.vert", "shaders/basic.frag");
    return shader;
}
