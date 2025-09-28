#include "GuidReader.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "json.hpp"
#include "PulseEngine/core/Meshes/primitive/Primitive.h"
#include "PulseEngine/core/Meshes/Mesh.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/core/FileManager/FileManager.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"

Entity *GuidReader::GetEntityFromGuid(std::size_t guid)
{
    static int count = 0;
    std::string name = "Entity_" + std::to_string(count++);
    Entity* entity = new Entity(name, glm::vec3(0), nullptr, MaterialManager::loadMaterial(std::string(ASSET_PATH) + "Materials/cube.mat"));
    
    nlohmann::json guidCollection;
    std::ifstream guidColFile(std::string(ASSET_PATH) +"Guid/guidCollectionEntities.puid");
    if(!guidColFile.is_open())
    {
        std::cout << "guid entity file not open" << std::endl;
        delete entity;
         return nullptr;
    }
    guidColFile >> guidCollection;

    if(guidCollection.contains(std::to_string(guid)))
    {
        nlohmann::json entityData;
        std::ifstream entityFile(std::string(ASSET_PATH) + std::string(guidCollection[std::to_string(guid)]));
        if(!entityFile.is_open())
        {
            std::cout << "entity data file not open" << std::endl;
            delete entity;
            return nullptr;
        }
        entityFile >> entityData;
        if(entityData.contains("Meshes"))
        {
            for (const auto& mesh : entityData["Meshes"])
            {
                std::cout << "adding mesh" << std::endl;
                std::size_t meshGuid = 0;
                try
                {
                    if (mesh.is_string())
                    {
                        meshGuid = std::stoull(mesh.get<std::string>());
                    }
                    else if (mesh.is_number_unsigned())
                    {
                        meshGuid = mesh.get<std::size_t>();
                    }
                    else
                    {
                        std::cerr << "Invalid mesh GUID format." << std::endl;
                        continue;
                    }
            
                    Mesh* msh = GetMeshFromGuid(meshGuid);
                    if (msh) entity->AddMesh(msh);
                }
                catch (const std::exception& e)
                {
                    std::cerr << "Error parsing mesh guid: " << e.what() << std::endl;
                }
            }
            
        }
        std::cout << "returning entity" << std::endl;
        return entity;
    }
    else
    {
        delete entity;
        return nullptr;
    }
}

Mesh *GuidReader::GetMeshFromGuid(std::size_t guid)
{
    return Primitive::Cube();
}

std::size_t GuidReader::InsertIntoCollection(const std::string &filePath)
{
    std::string collectionType = FileManager::GetCollectionByExtension(filePath);
    std::size_t guid = GenerateGUIDFromPath(filePath);

    std::string collectionPath = GUID_COLLECTION_PATH + collectionType;
    std::cout << collectionPath << std::endl;

    // Load existing collection
    nlohmann::json jsonData;

    std::ifstream inFile(collectionPath);
    if (inFile.is_open()) {
        // If file exists, read the existing JSON data
        inFile >> jsonData;
        inFile.close();
    } else {
        // If file doesn't exist, initialize an empty JSON object
        std::cerr << "File not found, creating new collection file." << std::endl;
        jsonData = nlohmann::json::object();  // Initialize an empty JSON object
    }

    // Insert or update entry
    jsonData[std::to_string(guid)] = filePath;

    // Rewrite the entire file (create or update the file)
    std::ofstream outFile(collectionPath);
    if (outFile.is_open()) {
        outFile << jsonData.dump(4); // 4 = pretty print with indentation
        outFile.close();
    } else {
        std::cerr << "Failed to open GUID collection file for writing." << std::endl;
    }

    return guid;
}
