#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm/glm.hpp>
#include "dllExport.h"

class Shader;

class MA_DLL_API Material
{
    public:
        Material(const std::string& name, Shader* shader) : name(name), shader(shader) {}

        Shader* GetShader();

        float roughness = 10.0f;
        float specular = 10.0f;
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

        void SetName(std::string name) {this->name = name;}
        const std::string GetName() const {return name; }
        
    
    private:
        Shader* shader;
        std::string name;
        
};

#endif