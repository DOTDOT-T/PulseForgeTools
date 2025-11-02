#include "EntityEditor.h"
#include "PulseEngineEditor/InterfaceEditor/InterfaceAPI/PulseInterfaceAPI.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/Math/MathUtils.h"
#include "PulseEngine/core/GUID/GuidReader.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/CustomScripts/ScriptsLoader.h"
#include "PulseEngine/core/Meshes/Mesh.h"
#include "PulseEngine/core/Meshes/RenderableMesh.h"
#include "PulseEngine/core/Meshes/StaticMesh.h"
#include "PulseEngine/core/Meshes/SkeletalMesh.h"
#include "PulseEngine/core/Material/Material.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "PulseEngine/API/EngineApi.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "PulseEngine/API/GameEntity.h"
#include "Shader.h"
#include "camera.h"
#include <windows.h>
#include "json.hpp"




std::string EntityEditor::GetName() const
{
    return std::string("Entity Editor");
}

std::string EntityEditor::GetVersion() const
{
    return std::string("v0.0.1");
}

void EntityEditor::Initialize()
{
    PulseInterfaceAPI::AddFunctionToFileClickedCallbacks(
        [this](const ClickedFileData& data) { this->NewFileClicked(data); }
    );
    forRender = new Shader(std::string(ASSET_PATH) + "shaders/basic.vert", std::string(ASSET_PATH) +  "shaders/basic.frag");
}

void EntityEditor::Shutdown()
{
}

void EntityEditor::Render()
{
    if (!selectedEntity)
    {
        std::cout << "[DEBUG] Render skipped: no selected entity\n";
        return;
    }

    if(!cam)
    {
        cam = new Camera();
    }
    std::vector<Entity*> entitiesToRender;
    std::cout << "[DEBUG] Rendering entity: " << selectedEntity->GetName() << std::endl;
    std::cout << " - Shader pointer: " << forRender << std::endl;
    // std::cout << " - Camera pointer: " << cam << std::endl;
    // std::cout << " - Mesh count: " << selectedEntity->GetMeshes().size() << std::endl;
    // std::cout << " - Material pointer: " << selectedEntity->GetMaterial() << std::endl;

    // if (selectedEntity)
    // {
    //     cam->Position = selectedEntity->GetPosition();
    //     cam->Position -= cam->Front * distCam; // Move the camera back a bit

    //     selectedEntity->SetRotation(PulseEngine::Vector3(0.0f, 0.0f, 0.0f)); // Reset rotation for better view
    //     entitiesToRender.push_back(selectedEntity);
    // }

    // std::string entName = "";

    // if(selectedEntity)
    // {
    //     entName = "- ";
    //     entName += selectedEntity->GetName();
    // }



    // PulseInterfaceAPI::OpenWindow(("Entity Editor " + entName + "###EntityEditor").c_str());
    //     PulseEngine::Vector2 windowSize = PulseInterfaceAPI::GetActualWindowSize();
    // PulseEngine::Vector2 oneWindow = PulseEngine::Vector2(windowSize.x / 2 - 20, windowSize.y - 65);
    // ManageCamera();

    //  PulseInterfaceAPI::SameLine();
    //  PulseInterfaceAPI::RenderCameraToInterface(&previewData, cam, "Entity Editor", PulseEngine::Vector2(windowSize.x * 0.5f - 20, windowSize.y - 65), entitiesToRender, forRender);
    // PulseInterfaceAPI::SameLine();
    // PulseInterfaceAPI::BeginChild("entity data", PulseEngine::Vector2(-1.0f, -1.0f), true);
    // PulseInterfaceAPI::BeginChild("Entity Properties", PulseEngine::Vector2(-1.0f, windowSize.y *0.5f - 65), true);
    // PulseInterfaceAPI::WriteText("Entity Properties");

    // if (selectedEntity)
    // {
    //     static bool scriptOpen = true;
    //     static bool meshesOpen = true;
    //     if(PulseInterfaceAPI::StartTreeNode("Scripts", &scriptOpen))
    //     {
    //         EntityScriptManager();
    //         PulseInterfaceAPI::EndTreeNode();
    //     }

    //     if(PulseInterfaceAPI::StartTreeNode("Meshes", &meshesOpen))
    //     {
    //         EntityMeshesManager();
    //         PulseInterfaceAPI::EndTreeNode();
    //     }

    //     Material* currentMaterial = selectedEntity->GetMaterial();
    //     if(currentMaterial)
    //     {
    //     PulseInterfaceAPI::AddMaterialPreview(currentMaterial, PulseEngine::Vector2(50.0f, 50.0f), "Material Preview");
    //     }

    //     if (currentMaterial != selectedEntity->GetMaterial())
    //     {
    //         selectedEntity->SetMaterial(currentMaterial);
    //     }
    // }


    // PulseInterfaceAPI::EndChild();

    // PulseInterfaceAPI::BeginChild("Entity specific data modifier", PulseEngine::Vector2(-1.0f, windowSize.y *0.5f - 65), true);
    // if (selectedData) 
    // {
    //     PulseInterfaceAPI::AddTransformModifierForMesh(selectedData, "Entity Transform Modifier");
    // }
    
    // PulseInterfaceAPI::EndChild();
    // if(PulseInterfaceAPI::Button("Add to Entity", PulseEngine::Vector2((windowSize.x * 0.3f - 20) * 0.5f, 0.0f)))
    // {
    //     isAddingToEntity = true;
    // }
    // PulseInterfaceAPI::SameLine();
    // if(PulseInterfaceAPI::Button("Save", PulseEngine::Vector2((windowSize.x * 0.3f - 20) * 0.5f, 0.0f)))
    // {
    //     SaveEntityToFile();
    // }

    // if(isAddingToEntity)
    // {
    //     PulseInterfaceAPI::OpenContextMenu("adding");
    // }

    // PulseInterfaceAPI::ShowContextMenu("adding",
    //     {
    //         {
    //             "Scripts",
    //             [&]() mutable
    //             {
    //                 PulseInterfaceAPI::OpenContextMenu("AddingScripts");
    //                 isAddingToEntity = false;
    //                     isSeekingScripts = true;
    //                 scriptsContextMenu.clear();

    //                 ContextMenuItem header;
    //                 header.label = "all scripts";
    //                 header.type = EditorWidgetComponent::TEXT;
    //                 header.style["color"]["r"] = 0.15f;
    //                 header.style["color"]["g"] = 0.15f;
    //                 header.style["color"]["b"] = 0.15f;
    //                 header.style["color"]["a"] = 1;

    //                 scriptsContextMenu.push_back(header);

    //                 header.type = EditorWidgetComponent::SEPARATOR;
    //                 scriptsContextMenu.push_back(header);


    //                 for (const auto &[name, createFunc] : ScriptsLoader::scriptMap)
    //                 {
    //                     ContextMenuItem newMenu;
    //                     newMenu.label = name;
    //                     newMenu.type = EditorWidgetComponent::SELECTABLE;
    //                     newMenu.onClick =[&]()
    //                     {
    //                         IScript *newScript = createFunc();

    //                         //let's create a guid from script name and the date it was created
    //                         std::string uniqueString = name + std::to_string(std::time(nullptr));
    //                         newScript->SetGUID(GenerateGUIDFromPath(uniqueString));
    //                         if (newScript)
    //                         {
    //                             selectedEntity->AddScript(newScript);
    //                         }
    //                         isSeekingScripts = false;
    //                     };
    //                     scriptsContextMenu.push_back(newMenu);
    //                 }
    //             },
    //             EditorWidgetComponent::SELECTABLE
            
    //         },
    //         {
    //             "Mesh",
    //             [&]() mutable
    //             {
    //                 PulseInterfaceAPI::OpenContextMenu("AddingMeshes");
    //                 isAddingToEntity = false;
    //                 isAddingMeshes = true;
    //                 meshesContextMenu.clear();

    //                 ContextMenuItem header;
    //                 header.label = "all meshes";
    //                 header.type = EditorWidgetComponent::TEXT;
    //                 header.style["color"]["r"] = 0.15f;
    //                 header.style["color"]["g"] = 0.15f;
    //                 header.style["color"]["b"] = 0.15f;
    //                 header.style["color"]["a"] = 1;

    //                 meshesContextMenu.push_back(header);

    //                 header.type = EditorWidgetComponent::SEPARATOR;
    //                 meshesContextMenu.push_back(header);

    //                 for (const auto& pr : GuidReader::GetAllAvailableFiles("guidCollectionMeshes.puid"))
    //                 {

    //                     EDITOR_LOG("first -> " << pr.first << " second -> " << pr.second)

    //                     ContextMenuItem mesh;
    //                     mesh.label = pr.second;
    //                     mesh.type = EditorWidgetComponent::SELECTABLE;
    //                     const std::string guidStr = pr.first;
    //                     const std::string meshName = pr.second;

    //                     mesh.onClick = [&, guidStr, meshName]() mutable
    //                     {
    //                         unsigned long long guidValue = 0;
    //                         try
    //                         {
    //                             guidValue = std::stoull(guidStr);
    //                         }
    //                         catch (const std::exception& e)
    //                         {
    //                             EDITOR_LOG("Invalid GUID string: " << guidStr << " (" << e.what() << ")");
    //                             return;
    //                         }
    //                         RenderableMesh *newMesh = GuidReader::GetMeshFromGuid((guidValue));
    //                         newMesh->SetGuid((guidValue));
    //                         newMesh->SetName(meshName);
    //                         if (newMesh)
    //                         {
    //                             selectedEntity->GetMeshes().push_back(newMesh);
    //                         }
    //                         isAddingMeshes = false;
    //                     };
    //                     meshesContextMenu.push_back(mesh);

    //                 }
    //             },
    //             EditorWidgetComponent::SELECTABLE
    //         }
    //     });

    // if(isSeekingScripts)
    // {
    //     PulseInterfaceAPI::OpenContextMenu("AddingScripts");
    // }
    // if(isAddingMeshes)
    // {
    //     PulseInterfaceAPI::OpenContextMenu("AddingMeshes");
    // }

    // PulseInterfaceAPI::ShowContextMenu("AddingScripts",scriptsContextMenu);
    // PulseInterfaceAPI::ShowContextMenu("AddingMeshes",meshesContextMenu);
    
    // PulseInterfaceAPI::EndChild();
    // PulseInterfaceAPI::CloseWindow();
}

void EntityEditor::SaveEntityToFile()
        {
            if (selectedEntity)
            {
                nlohmann::json_abi_v3_12_0::json entityData;
                entityData["Guid"] = std::to_string(selectedEntity->GetGuid());

                for (const auto &mesh : selectedEntity->GetMeshes())
                {
                    nlohmann::json meshJson;
                    meshJson["Guid"] = std::to_string(mesh->GetGuid());
                    meshJson["Position"] = {mesh->position.x, mesh->position.y, mesh->position.z};
                    meshJson["Rotation"] = {mesh->rotation.x, mesh->rotation.y, mesh->rotation.z};
                    meshJson["Scale"] = {mesh->scale.x, mesh->scale.y, mesh->scale.z};
                    meshJson["Name"] = mesh->GetName();
                    meshJson["MeshPath"] = mesh->GetName();
                    entityData["Meshes"].push_back(meshJson);
                }

                for (const auto &script : selectedEntity->GetScripts())
                {
                    nlohmann::json scriptJson;
                    scriptJson["Name"] = script->GetName();
                    scriptJson["Guid"] = script->GetGUID();
                    entityData["Scripts"].push_back(scriptJson);
                }

                entityData["Material"] = selectedEntity->GetMaterial()->guid;

                std::string filePath = selectedEntity->GetName();
                std::ofstream file(filePath);
                if (file.is_open())
                {
                    file << entityData.dump(4); // Pretty print with 4 spaces
                    file.close();
                    std::cout << "Entity saved to " << filePath << std::endl;
                }
                else
                {
                    std::cerr << "Failed to open file for saving: " << filePath << std::endl;
                }
            }
            else
            {
                std::cerr << "No entity selected to save." << std::endl;
            }
        }

void EntityEditor::ManageCamera()
{
    if(PulseInterfaceAPI::IsCurrentWindowFocused())
    {
        if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        {
            distCam += PulseInterfaceAPI::MouseDelta().y * 0.1f;
        }
        if (selectedEntity && (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
        {
            cam->Yaw += PulseInterfaceAPI::MouseDelta().x * 0.1f;
            cam->Pitch -= PulseInterfaceAPI::MouseDelta().y * 0.1f;
            if (cam->Pitch > 89.0f) cam->Pitch = 89.0f;
            if (cam->Pitch < -89.0f) cam->Pitch = -89.0f;

            cam->Position = PulseEngine::MathUtils::RotateAround(selectedEntity->GetPosition(), cam->Yaw, cam->Pitch, distCam);
            cam->Front = (selectedEntity->GetPosition() - cam->Position).Normalized();

        }
    }
}

void EntityEditor::EntityMeshesManager()
{
    auto &meshes = selectedEntity->GetMeshes();
    int counter = 0;

    for (auto it = meshes.rbegin(); it != meshes.rend(); )
    {
        auto mesh = *it;

        // Give each child and popup a unique ID
        std::string childId = "MeshProperties##" + std::to_string(counter);
        std::string popupId = "MeshContextMenu##" + std::to_string(counter);

        // Selectable mesh entry
        if (PulseInterfaceAPI::Selectable(("mesh " + std::to_string(counter) + " " + mesh->GetName()).c_str()))
        {
            selectedData = mesh;
            PulseInterfaceAPI::OpenContextMenu(popupId.c_str());
        }

        // Show context menu for this mesh
        PulseInterfaceAPI::ShowContextMenu(popupId.c_str(),
        {
            {
                "Delete",
                [&, it, mesh]() mutable
                {
                    it = decltype(it)(meshes.erase((++it).base()));
                    delete mesh;
                },
                EditorWidgetComponent::SELECTABLE
            }
        });

        ++it;
        counter++;
    }
}

void EntityEditor::AddMeshToEntity()
{
    static bool isSelected = false;

    if (PulseInterfaceAPI::Button("Add Mesh", PulseEngine::Vector2(-1.0f, 0.0f)))
    {
        isSelected = true;
    }
    if (isSelected)
    {
        if (PulseInterfaceAPI::BeginCombo("Choose a mesh", "mesh"))
        {
            for (const auto pr : GuidReader::GetAllAvailableFiles("guidCollectionMeshes.puid"))
            {
                if (PulseInterfaceAPI::Selectable(pr.second, isSelected))
                {
                    RenderableMesh *newMesh = GuidReader::GetMeshFromGuid(std::stoull(pr.first));
                    newMesh->SetGuid(std::stoull(pr.first));
                    newMesh->SetName(pr.second);
                    if (newMesh)
                    {
                        selectedEntity->GetMeshes().push_back(newMesh);
                    }
                    isSelected = false;
                }
            }
            PulseInterfaceAPI::EndCombo();
        }
    }
}

void EntityEditor::EntityScriptManager()
{
    auto& scripts = selectedEntity->GetScripts();

    static bool hasOpenContextual = false;
    static IScript* currentScript = nullptr;
    static int currentIndex = -1;

    for (size_t i = 0; i < scripts.size(); /* no ++i here */)
    {
        IScript* script = scripts[i];



        hasOpenContextual = PulseInterfaceAPI::Selectable(std::string(script->GetName()) + "###Script" + std::to_string(i));

        if (hasOpenContextual)
        {
            PulseInterfaceAPI::OpenContextMenu("Entity Script context menu");
            hasOpenContextual = false;
            currentScript = script;
            currentIndex = static_cast<int>(i);
        }
        if(currentScript == script)
        {
            EDITOR_LOG("Opening contextual menu for script: " + std::string(currentScript->GetName()));
            PulseInterfaceAPI::ShowContextMenu("Entity Script context menu",
            {
                {
                    "Delete",
                    [&]
                    {
                        hasOpenContextual = false;
                        scripts.erase(scripts.begin() + currentIndex);
                        currentIndex = -1;
                        currentScript = nullptr;
                    },
                    EditorWidgetComponent::SELECTABLE
                }
            });

        }

        ++i; // only increment if we didn’t erase
        
    }
}


void EntityEditor::AddScriptToEntity()
{
    static bool isSelected = false;


    if (PulseInterfaceAPI::BeginCombo("Choose a script", "Select a script"))
    {
        for (const auto &[name, createFunc] : ScriptsLoader::scriptMap)
        {
            if (PulseInterfaceAPI::Selectable(name.c_str(), isSelected))
            {
                IScript *newScript = ScriptsLoader::GetScriptFromCallName(name);

                //let's create a guid from script name and the date it was created
                std::string uniqueString = name + std::to_string(std::time(nullptr));
                newScript->SetGUID(GenerateGUIDFromPath(uniqueString));
                if (newScript)
                {
                    selectedEntity->AddScript(newScript);
                }
                isSelected = false;
            }
        }
        PulseInterfaceAPI::EndCombo();
    }
    
}

void EntityEditor::NewFileClicked(const ClickedFileData &data)
{

    std::string fullPath = data.name.string();
    if (fullPath.size() >= 8 && fullPath.substr(fullPath.size() - 8) == ".pEntity")
    {
        
        if(PulseInterfaceAPI::Selectable("Entity Editor"))
        {      
            PulseInterfaceAPI::ChangeWindowState(this, true);
            if(selectedEntity)
            {
                delete selectedEntity;
                selectedEntity = nullptr;
            }  
            std::ifstream file(fullPath);
            nlohmann::json_abi_v3_12_0::json entityData;
            file >> entityData;
            file.close();
            selectedEntity = new Entity(fullPath, PulseEngine::Vector3(0.0f, 0.0f, 0.0f));
            GuidReader::GetEntityFromJson(entityData, selectedEntity);
            if (selectedEntity)
            {
                selectedEntity->SetName((data.name).string());
                // selectedEntity->SetMaterial(MaterialManager::loadMaterial(std::string(ASSET_PATH) + "Materials/cube.mat"));
                selectedEntity->SetScale(PulseEngine::Vector3(1.0f, 1.0f, 1.0f));
                std::cout << "Selected entity: " << selectedEntity->GetName() << std::endl;
            }
            else
            {
                std::cerr << "Failed to load entity " << std::endl;
            }
        }

        if (PulseInterfaceAPI::Selectable("Add to scene"))
        {
            std::string instantiatePath = fullPath;
            std::cout << fullPath << std::endl;
            // Replace backslashes with forward slashes
            size_t pos = instantiatePath.find("PulseEngineEditor\\");
            if (pos != std::string::npos) {
                instantiatePath.erase(pos, std::string("PulseEngineEditor\\").length());
            }
            PulseEngine::GameEntity::Instantiate(instantiatePath, PulseEngine::Vector3(0.0f, 0.0f, 0.0f), PulseEngine::Vector3(0.0f, 0.0f, 0.0f), PulseEngine::Vector3(1.0f, 1.0f, 1.0f));
        }
    }
}

extern "C" __declspec(dllexport) IModule* CreateModule()
{
    return new EntityEditor();
}