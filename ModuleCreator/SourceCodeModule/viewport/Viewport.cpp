#include "Viewport.h"

#include  "PulseEngine/ModuleLoader/IModuleInterface/IModuleInterface.h"
#include  "PulseEngine/ModuleLoader/IModule/IModule.h"
#include "PulseEngineEditor/InterfaceEditor/InterfaceAPI/PulseInterfaceAPI.h"
#include "PulseEngine/core/Graphics/IGraphicsApi.h"
#include "PulseEngine/core/Graphics/OpenGLAPI/OpenGLApi.h"
#include "PulseEngine/core/PulseEngineBackend.h"
#include <WinUser.h>

extern "C" __declspec(dllexport) IModule* CreateModule()
{
    return new Viewport();
}

std::string Viewport::GetName() const
{
    return std::string("viewport");
}

std::string Viewport::GetVersion() const
{
    return std::string("0.0.1");
}

void Viewport::Initialize()
{
}

void Viewport::Shutdown()
{
}

void Viewport::Render()
{
    PulseInterfaceAPI::OpenWindow("Viewport");

    SceneRenderer();
    if(PulseInterfaceAPI::Button("Renderer", PulseEngine::Vector2(0.0f,0.0f)))
    {

    }
    PulseInterfaceAPI::CloseWindow();
}

void Viewport::SceneRenderer()
{
    if (PulseInterfaceAPI::IsCurrentWindowFocused() && (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
    {
        if (GetAsyncKeyState('W') & 0x8000)
            PulseEngineInstance->GetActiveCamera()->ProcessKeyboard(FORWARD, PulseEngineInstance->GetDeltaTime());
        if (GetAsyncKeyState('S') & 0x8000)
            PulseEngineInstance->GetActiveCamera()->ProcessKeyboard(BACKWARD, PulseEngineInstance->GetDeltaTime());
        if (GetAsyncKeyState('A') & 0x8000)
            PulseEngineInstance->GetActiveCamera()->ProcessKeyboard(LEFT, PulseEngineInstance->GetDeltaTime());
        if (GetAsyncKeyState('D') & 0x8000)
            PulseEngineInstance->GetActiveCamera()->ProcessKeyboard(RIGHT, PulseEngineInstance->GetDeltaTime());
        PulseEngineInstance->GetActiveCamera()->ProcessMouseMovement(PulseInterfaceAPI::MouseDelta().x, -PulseInterfaceAPI::MouseDelta().y);
    }

    OpenGLAPI *openGLAPI = dynamic_cast<OpenGLAPI *>(PulseEngineInstance->graphicsAPI);

    PulseEngine::Vector2 windowSize = PulseInterfaceAPI::GetActualWindowSize();
    windowSize.x -= 100.0f;
    windowSize.y -= 100.0f;

    PulseInterfaceAPI::RenderCameraToInterface(&previewData, PulseEngineInstance->GetActiveCamera(), "Viewport", windowSize, PulseEngineInstance->entities);
}
