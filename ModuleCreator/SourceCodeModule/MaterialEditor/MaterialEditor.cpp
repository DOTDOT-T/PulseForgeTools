#include "MaterialEditor.h"
#include "PulseEngineEditor/InterfaceEditor/InterfaceAPI/PulseInterfaceAPI.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/Math/MathUtils.h"
#include "PulseEngine/core/GUID/GuidReader.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/CustomScripts/ScriptsLoader.h"
#include "PulseEngine/core/Meshes/Mesh.h"
#include "PulseEngine/core/Material/Material.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "PulseEngine/core/Meshes/primitive/Primitive.h"
#include "PulseEngine/core/FileManager/FileReader/FileReader.h"
#include "Shader.h"
#include "camera.h"
#include <windows.h>
#include "json.hpp"




std::string MaterialEditor::GetName() const
{
    return std::string("Material Editor");
}

std::string MaterialEditor::GetVersion() const
{
    return std::string("v0.0.1");
}

void MaterialEditor::Initialize()
{
    std::cout << "WE INIT THE MATERIAL" << std::endl;
    PulseInterfaceAPI::AddFunctionToFileClickedCallbacks(
        [this](const ClickedFileData& data) { this->NewFileClicked(data); }
    );
    
    cam = new Camera();
    materialSelected = MaterialManager::loadMaterial("Materials/StandardLit.mat");
    materialRenderer = new Entity("MaterialRenderer", PulseEngine::Vector3(0.0f,0.0f,0.0f), Primitive::Cube(), materialSelected);
    forRender = new Shader("PulseEngineEditor/shaders/basic.vert",  "PulseEngineEditor/shaders/basic.frag");
}

void MaterialEditor::Shutdown()
{
}

void MaterialEditor::Render()
{
    std::vector<Entity*> entitiesToRender;

    cam->Position = materialRenderer->GetPosition();
    cam->Position -= cam->Front * distCam; // Move the camera back a bit

    materialRenderer->SetRotation(PulseEngine::Vector3(0.0f, 0.0f, 0.0f)); // Reset rotation for better view
    entitiesToRender.push_back(materialRenderer);
    
    std::string matName = "";

    if(materialSelected)
    {
        matName = "- ";
        matName += materialSelected->GetName();
    }

    PulseInterfaceAPI::OpenWindow(("Material Editor " + matName + "###MaterialEditor").c_str());
    ManageCamera();
    PulseEngine::Vector2 windowSize = PulseInterfaceAPI::GetActualWindowSize();
    PulseEngine::Vector2 viewportSize = PulseEngine::Vector2(windowSize.x / 2, windowSize.y - 60.0f);

    PulseInterfaceAPI::RenderCameraToInterface(&previewData, cam, "Material Editor", viewportSize, entitiesToRender, forRender);

    PulseInterfaceAPI::SameLine();

    PulseInterfaceAPI::BeginChild("Material base data", PulseEngine::Vector2(-1.0f, PulseInterfaceAPI::GetActualWindowSize().y - 60), true);

    PulseInterfaceAPI::WriteText("Material data");

    PulseInterfaceAPI::Separator();
    
    if(PulseInterfaceAPI::DragFloat("Specular", &materialSelected->specular, 0.01f, 0.0f, 1.0f))
    {
        
    }

    if(PulseInterfaceAPI::DragFloat3("Color", &materialSelected->color.x, 0.01f, 0.0f, 1.0f))
    {
        
    }

    std::vector<std::string> textureTypes = {"albedo", "normal", "roughness"};
    for (const auto& type : textureTypes)
    {
        TextureSelector(type);
    }

    if(PulseInterfaceAPI::Button("Save Material", PulseEngine::Vector2(-1.0f, 0.0f)))
    {
        if (materialSelected)
        {
            std::string filePath = materialSelected->GetPath();
            nlohmann::json materialData;
            materialData["name"] = materialSelected->GetName();
            materialData["guid"] = materialSelected->guid;
            materialData["specular"] = materialSelected->specular;
            materialData["color"] = {materialSelected->color.x, materialSelected->color.y, materialSelected->color.z};

            for (const auto& texture : materialSelected->GetAllTextures())
            {
                if (texture.second && !texture.second->GetPath().empty())
                {
                    materialData[texture.first] = texture.second->GetPath();
                }
            }

            std::ofstream file(std::string(ASSET_PATH) + filePath);
            if (file.is_open())
            {
                file << materialData.dump(4); // Pretty print with 4 spaces
                file.close();
                std::cout << "Material saved to " << filePath << std::endl;
            }
        }
    }


    PulseInterfaceAPI::EndChild();

    PulseInterfaceAPI::CloseWindow();
}
void MaterialEditor::TextureSelector(const std::string &textureName)
{
    PulseInterfaceAPI::BeginChild("Texture Selector#xx" + textureName, PulseEngine::Vector2(0.0f, 160.0f), true);
    if (PulseInterfaceAPI::BeginCombo(textureName + " Texture", materialSelected->GetTexture(textureName) ? materialSelected->GetTexture(textureName)->GetPath() : "None"))
    {
        for (const auto pr : GuidReader::GetAllAvailableFiles("guidCollectionTextures.puid"))
        {
            if (PulseInterfaceAPI::Selectable(pr.second, true))
            {
                Texture *albedoTexture = new Texture(pr.second);
                if (albedoTexture)
                {
                    materialSelected->SetTexture(textureName, std::shared_ptr<Texture>(albedoTexture));
                }
            }
        }
        PulseInterfaceAPI::EndCombo();
    }
    else if (materialSelected->GetTexture(textureName))
    {
        PulseInterfaceAPI::Image(materialSelected->GetTexture(textureName)->id, PulseEngine::Vector2(52.0f, 52.0f),
                                 PulseEngine::Vector2(0.0f, 0.0f), PulseEngine::Vector2(1.0f, 1.0f));
    }
    PulseInterfaceAPI::EndChild();
}
void MaterialEditor::ManageCamera()
{
    if(PulseInterfaceAPI::IsCurrentWindowFocused())
    {
        if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        {
            distCam += PulseInterfaceAPI::MouseDelta().y * 0.1f;
        }
        if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000))
        {
            cam->Yaw += PulseInterfaceAPI::MouseDelta().x * 0.1f;
            cam->Pitch -= PulseInterfaceAPI::MouseDelta().y * 0.1f;
            if (cam->Pitch > 89.0f) cam->Pitch = 89.0f;
            if (cam->Pitch < -89.0f) cam->Pitch = -89.0f;

            cam->Position = PulseEngine::MathUtils::RotateAround(materialRenderer->GetPosition(), cam->Yaw, cam->Pitch, distCam);
            cam->Front = (materialRenderer->GetPosition() - cam->Position).Normalized();
        }
    }
}
void MaterialEditor::NewFileClicked(const ClickedFileData &data)
{
    std::string fullPath = data.name.string();
    // Remove "PulseEngineEditor/" from fullPath if present
    std::string prefix = "PulseEngineEditor\\";
    if (fullPath.rfind(prefix, 0) == 0) {
        fullPath = fullPath.substr(prefix.length());
    }
    prefix = "PulseEngineEditor/";
    if (fullPath.rfind(prefix, 0) == 0) {
        fullPath = fullPath.substr(prefix.length());
    }

    if (fullPath.size() >= 4 && fullPath.substr(fullPath.size() - 4) == ".mat")
    {
        if(PulseInterfaceAPI::Selectable("Material Editor"))
        {
            PulseInterfaceAPI::ChangeWindowState(this, true);
            materialSelected = MaterialManager::loadMaterial(fullPath);
            materialRenderer->SetMaterial(materialSelected);
            std::cout << "new material selected " << fullPath << std::endl;
        }
    }

}
extern "C" __declspec(dllexport) IModule* CreateModule()
{
    return new MaterialEditor();
}