#include "SceneLoader.h"
#include "json.hpp"
#include "PulseEngine/core/GUID/GuidReader.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/PulseEngineBackend.h"


void SceneLoader::LoadScene(const std::string &mapName, PulseEngineBackend* backend)
{
    std::ifstream scene(mapName);
    if (!scene.is_open())
    {
        std::cout << "Couldn't open map " << mapName << std::endl;
        return;
    }
    backend->ClearScene();

    nlohmann::json sceneData;
    scene >> sceneData;

    backend->SetWindowName(sceneData["sceneName"]);

    for (const auto& entityData : sceneData["entities"])
    {
        glm::vec3 position;
        glm::vec3 rotation;

        std::cout << "creating a new entity" << std::endl;

        position.x = entityData["position"][0].get<float>();
        position.y = entityData["position"][1].get<float>();
        position.z = entityData["position"][2].get<float>();

        rotation.x = entityData["rotation"][0].get<float>();
        rotation.y = entityData["rotation"][1].get<float>();
        rotation.z = entityData["rotation"][2].get<float>();

        std::size_t guid = entityData["guid"].get<std::size_t>();
        nlohmann::json entityDocData;

        Entity* entity = GuidReader::GetEntityFromGuid(guid);
        if(!entity) continue;
        entity->SetGuid(guid);
        std::cout << "everything fine here i guess " << entity << std::endl;
        entity->SetPosition(position);
        entity->SetRotation(rotation);

        backend->entities.push_back(entity);
    }
}

std::vector<std::string> SceneLoader::GetSceneFiles(const std::string &directory)
{
    std::vector<std::string> sceneFiles;
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".pmap")
        {
            sceneFiles.push_back(entry.path().string());
        }
    }
    return sceneFiles;
}

void SceneLoader::SaveSceneToFile(const std::string &mapName, PulseEngineBackend *backend)
{
    nlohmann::json sceneData;

    // Set the scene name
    sceneData["sceneName"] = mapName;

    // Save entities
    for (const auto& entity : backend->entities)
    {
        nlohmann::json entityData;

        // Save position
        glm::vec3 position = entity->GetPosition();
        entityData["position"] = { position.x, position.y, position.z };

        // Save rotation
        glm::vec3 rotation = entity->GetRotation();
        entityData["rotation"] = { rotation.x, rotation.y, rotation.z };

        // Save GUID
        entityData["guid"] = entity->GetGuid();

        sceneData["entities"].push_back(entityData);
    }

    // Write to file
    std::ofstream sceneFile(std::string(ASSET_PATH) + "Scenes/" + std::string(mapName) + ".pmap");
    if (!sceneFile.is_open())
    {
        std::cout << "Couldn't open file to save map " << mapName << std::endl;
        return;
    }

    sceneFile << sceneData.dump(4); // Pretty print with 4 spaces
    sceneFile.close();
}
