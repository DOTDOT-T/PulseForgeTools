#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include "Common/common.h"
#include "dllExport.h"

class Mesh;
class Shader;
class Material;

class MA_DLL_API Entity
{
    public:
        Entity(const std::string& name, const glm::vec3& position, Mesh* mesh, Material* material);
        ~Entity() {}

        void SetPosition(const glm::vec3& position) { this->position = position; UpdateModelMatrix();}
        void SetRotation(const glm::vec3& rotation) { this->rotation = rotation; UpdateModelMatrix();}
        void SetScale(const glm::vec3& scale) {this->scale = scale; UpdateModelMatrix();}

        void DrawEntity() const;

        void UseShader() const;

        void SimplyDrawMesh() const;

        Material* GetMaterial() {return material; }
        void AddMesh(Mesh* mesh) {meshes.push_back(mesh); std::cout << "new mesh added" << std::endl;}

        const glm::vec3& GetPosition() const {return position;}
        const glm::vec3& GetRotation() const {return rotation;}
        const glm::vec3& GetScale() const {return scale; }
        const glm::mat4& GetMatrix() const { return modelMatrix; }
        const std::size_t& GetGuid() const {return guid;}
        void SetGuid(std::size_t g) {guid = g;}
        std::string GetName() const {return name;}
    private:
        std::string name;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 modelMatrix;

        Material* material;
        std::size_t guid;

        std::vector<Mesh*> meshes;

        void UpdateModelMatrix();
};

#endif