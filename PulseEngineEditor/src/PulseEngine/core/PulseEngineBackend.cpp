#include "PulseEngineBackend.h"
#include "Common/common.h"
#include "camera.h"
#include "PulseEngine/core/Inputs/Mouse.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/Material/Material.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "json.hpp"
#include <fstream>
#include "PulseEngine/core/Meshes/primitive/Primitive.h"
#include "PulseEngine/core/Material/MaterialManager.h"
#include "PulseEngine/core/GUID/GuidReader.h"
#include "PulseEngine/core/SceneLoader/SceneLoader.h"
#include "PulseEngine/core/Lights/Lights.h"
#include "PulseEngine/core/FileManager/FileManager.h"
#include "PulseEngine/core/Lights/DirectionalLight/DirectionalLight.h"

#include "dllExport.h"

Camera* PulseEngineBackend::activeCamera = new Camera();


PulseEngineBackend::PulseEngineBackend()
{
}

int PulseEngineBackend::Initialize()
{
    std::cout << "init" << std::endl;
    windowContext = new WindowContext();
    activeCamera = new Camera();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    windowContext->SetGLFWWindow(glfwCreateWindow(1920, 1080, GetWindowName("editor").c_str(), nullptr, nullptr));
    if (!windowContext->GetGLFWWindow())
    {
        std::cerr << "Erreur : création de la fenêtre GLFW échouée !" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(windowContext->GetGLFWWindow());
    glfwSetFramebufferSizeCallback(windowContext->GetGLFWWindow(), FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Erreur : échec de l'initialisation de GLAD !" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);


    lights.push_back(new DirectionalLight(1.0f, 10.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f, 3.0f, 0.0f), glm::vec3(1.0f), 10, 1));
    lights.back()->InitShadowMap(2048);

    shadowShader = new Shader(std::string(ASSET_PATH) + "shaders/depthMap/Depth.vert", std::string(ASSET_PATH) + "shaders/depthMap/Depth.frag");
    debugShader = new Shader(std::string(ASSET_PATH) +"shaders/debug.vert", std::string(ASSET_PATH) + "shaders/debug.frag");
    
    // === insert base item to the collection ===
    GuidReader::InsertIntoCollection("Entities/simpleActor.pEntity");    
    GuidReader::InsertIntoCollection("primitiveCube.pmesh");    

    // Create texture to render to
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fboWidth, fboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create framebuffer
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

    // Create Renderbuffer for depth and stencil
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fboWidth, fboHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return 0;
}


void PulseEngineBackend::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void PulseEngineBackend::SetWindowName(const std::string &location)
{
    glfwSetWindowTitle(windowContext->GetGLFWWindow(), GetWindowName(location).c_str());
}

bool PulseEngineBackend::IsRunning()
{
    return !glfwWindowShouldClose(GetWindowContext()->GetGLFWWindow());
}

void PulseEngineBackend::PollEvents()
{

    glfwSetCursorPosCallback(GetWindowContext()->GetGLFWWindow(), MouseInput::MouseCallback);
    glfwSetScrollCallback(GetWindowContext()->GetGLFWWindow(), MouseInput::ScrollCallback);

    ProcessInput(GetWindowContext()->GetGLFWWindow());
    glfwSetInputMode(GetWindowContext()->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);


}

void PulseEngineBackend::Update()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    view = GetActiveCamera()->GetViewMatrix();
    projection = glm::perspective(glm::radians(GetActiveCamera()->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    mapLoading -= deltaTime;

    // Animate the light position
    static float timeAccumulator = 0.0f;
    timeAccumulator += deltaTime;

    for (auto& light : lights) {
        light->position.x = 3.0f + sin(timeAccumulator) * 2.0f; // Oscillate along x-axis
        light->position.z = cos(timeAccumulator) * 2.0f;       // Oscillate along z-axis

        // Update light space matrix for shadow mapping
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
        glm::mat4 lightView = glm::lookAt(light->position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        light->lightSpaceMatrix = lightProjection * lightView;
    }
}

void PulseEngineBackend::Render()
{
    EDITOR_ONLY(
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, fboWidth, fboHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    )
    IN_GAME_ONLY(
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1920, 1080);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.8f, 0.8f, 0.6f, 1.0f);
    )

    for(Entity* entity : entities)
    {
        if(!entity) 
        {
            std::cerr <<"entity not valid!" << std::endl;
            continue;
        }
        if (!entity->GetMaterial())
        {
            std::cerr << "Entity has no material!" << std::endl;
            continue;
        }
        
        if (!entity->GetMaterial()->GetShader())
        {
            std::cerr << "Material has no shader!" << std::endl;
            continue;
        }
        std::cout << "Rendering entity: " << entity->GetName() << std::endl;
        entity->UseShader();
        entity->GetMaterial()->GetShader()->SetMat4("projection", projection);
        entity->GetMaterial()->GetShader()->SetMat4("view", view);
        entity->GetMaterial()->GetShader()->SetVec3("viewPos", GetActiveCamera()->Position);

        std::vector<int> shadowCasterIndices;
        for (unsigned int i = 0; i < lights.size(); i++)
        {            
            lights[i]->BindToShader(*entity->GetMaterial()->GetShader(), i);
            if (lights[i]->castsShadow)
            {
                int casterIndex = static_cast<int>(shadowCasterIndices.size());
                shadowCasterIndices.push_back(i);
        
                std::string matName = "lightSpaceMatrices[" + std::to_string(casterIndex) + "]";
                entity->GetMaterial()->GetShader()->SetMat4(matName.c_str(), lights[i]->lightSpaceMatrix);
        
                glActiveTexture(GL_TEXTURE0 + casterIndex);
                glBindTexture(GL_TEXTURE_2D, lights[i]->depthMapTex);
        
                std::string texName = "shadowMaps[" + std::to_string(casterIndex) + "]";
                entity->GetMaterial()->GetShader()->SetInt(texName.c_str(), casterIndex); // matches GL_TEXTURE0 + i    
            }
        }

        entity->GetMaterial()->GetShader()->SetInt("lightCount", lights.size());
        entity->GetMaterial()->GetShader()->SetInt("shadowCasterCount", static_cast<int>(shadowCasterIndices.size()));
        entity->GetMaterial()->GetShader()->SetIntArray("shadowCasterIndices", shadowCasterIndices.data(), static_cast<int>(shadowCasterIndices.size()));

        entity->DrawEntity();
    }

    EDITOR_ONLY(
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Go back to default framebuffer
        glViewport(0, 0, 1920, 1080); // Reset to default screen size
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
    )
}

void PulseEngineBackend::RenderShadow()
{    
    for (int i = 0; i < lights.size(); ++i)
    {
        lights[i]->RenderShadowMap(*shadowShader, *this);
    }
}

void PulseEngineBackend::Shutdown()
{    
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(GetWindowContext()->GetGLFWWindow());
    glfwTerminate();
}

void PulseEngineBackend::ClearScene()
{
    for(auto ent : entities)
    {
        delete ent;
    }
    entities.clear();
}
void PulseEngineBackend::DeleteEntity(Entity *entity)
{
    auto it = std::find(entities.begin(), entities.end(), entity);
    if (it != entities.end())
    {
        delete *it;
        entities.erase(it);
    }
}

void PulseEngineBackend::ProcessInput(GLFWwindow* window)
{
    static bool loadOnce = true;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) activeCamera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) activeCamera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) activeCamera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) activeCamera->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        if(!loadOnce) return;
        loadOnce = false;
        SceneLoader::LoadScene(std::string(ASSET_PATH) + "Scenes/CZEFGZEOUFHDZOEIDYO.pmap", this);
    }
}

glm::vec3 PulseEngineBackend::CalculateLighting(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec3 &viewPos, const LightData& light)
{
    // Calculate the direction from the object to the light
    glm::vec3 lightDir = glm::normalize(light.position - position);

    // Diffuse lighting: Lambertian reflectance (dot product of normal and light direction)
    float diff = glm::max(glm::dot(normal, lightDir), 0.0f);

    // Specular lighting: Blinn-Phong reflection model
    glm::vec3 reflectDir = glm::reflect(-lightDir, normal);
    glm::vec3 viewDir = glm::normalize(viewPos - position);
    float spec = pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), 32); // 32 is the shininess

    // Apply attenuation (optional for point lights)
    float distance = glm::length(light.position - position);
    float attenuation = 1.0f / (1.0f + light.attenuation * distance * distance);

    // Final color calculation (ambient + diffuse + specular)
    glm::vec3 ambient = 0.1f * light.color; // Ambient light
    glm::vec3 diffuse = diff * light.color * light.intensity;
    glm::vec3 specular = spec * light.color * light.intensity;

    // Combine all lighting effects, considering attenuation for point lights
    glm::vec3 finalColor = (ambient + diffuse + specular) * attenuation;
    
    return finalColor;
}


