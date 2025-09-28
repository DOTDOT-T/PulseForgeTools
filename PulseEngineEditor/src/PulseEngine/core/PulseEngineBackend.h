#ifndef PULSEENGINE_H
#define PULSEENGINE_H

#include <string>
#include "Common/common.h"
#include "dllExport.h"

#define GUID_COLLECTION_PATH std::string(ASSET_PATH) + "Guid/"


class WindowContext;
class Entity;
class LightData;
class Camera;
class DirectionalLight;
class InterfaceEditor;

class MA_DLL_API PulseEngineBackend
{
public:
    friend InterfaceEditor;

    PulseEngineBackend();
    ~PulseEngineBackend() {}

    // === main functions Engine ===
    int Initialize();  // Fonction exportée
    bool IsRunning();  // Fonction exportée
    void PollEvents();  // Fonction exportée
    void Update();  // Fonction exportée
    void Render();  // Fonction exportée
    void RenderShadow();  // Fonction exportée
    void Shutdown();  // Fonction exportée

    void ClearScene();
    void DeleteEntity(Entity* entity);

    // === getters ===
    WindowContext* GetWindowContext() { return windowContext; }

    // === static functions ===
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static Camera* GetActiveCamera() {return activeCamera;}


    std::vector<Entity*> entities;
    std::vector<DirectionalLight*> lights;



    std::string GetWindowName(const std::string& location) {return engine + " | " + version +  " | " + location +  " | " + devMonth + " | " + company;}
    void SetWindowName(const std::string& location);
private:
    WindowContext* windowContext = nullptr;

    int width = 800;
    int height = 600;
    std::string engine = "Pulse Engine";
    std::string version = "V0.0.3";
    std::string devMonth = "April 2025";
    std::string company = "Pulse Software";

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float mapLoading = 0.0f;

    static Camera* activeCamera;

    glm::mat4 view;
    glm::mat4 projection;

    Shader* shadowShader;
    Shader* debugShader;

    GLuint fbo, fboTexture, rbo;
    int fboWidth = 1024;
    int fboHeight = 720;


    void ProcessInput(GLFWwindow* window);
    glm::vec3 CalculateLighting(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& viewPos, const LightData& light);

};

#endif
