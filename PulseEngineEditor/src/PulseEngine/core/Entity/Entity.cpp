#include "Entity.h"
#include "PulseEngine/core/Meshes/Mesh.h"
#include "shader.h"
#include "PulseEngine/core/Material/Material.h"
#include "Common/common.h"

Entity::Entity(const std::string &name, const glm::vec3 &position, Mesh* mesh, Material* material) :name(name), position(position), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), material(material)
{
    if(mesh) meshes.push_back(mesh);
    UpdateModelMatrix();
}

void Entity::UpdateModelMatrix()
{
    modelMatrix = glm::mat4(1.0f); // Initialize to identity matrix
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
}

void Entity::DrawEntity() const
{
    material->GetShader()->SetMat4("model", GetMatrix());
    material->GetShader()->SetFloat("specularStrength", material ? material->specular : 1.0f);
    material->GetShader()->SetVec3("objectColor", material ? material->color : glm::vec3(0.5f));
    SimplyDrawMesh();
}

void Entity::UseShader() const
{
    material->GetShader()->Use();
}

void Entity::SimplyDrawMesh() const
{
    for (const auto &mesh : meshes)
    {
        mesh->Draw();
    }
}
