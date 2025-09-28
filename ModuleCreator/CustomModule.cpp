#include "CustomModule.h"

#include  "PulseEngine/ModuleLoader/IModuleInterface/IModuleInterface.h"
#include  "PulseEngine/ModuleLoader/IModule/IModule.h"
#include "PulseEngineEditor/InterfaceEditor/InterfaceAPI/PulseInterfaceAPI.h"
#include "PulseEngine/core/Graphics/IGraphicsApi.h"
#include "PulseEngine/core/Graphics/OpenGLAPI/OpenGLApi.h"
#include "PulseEngine/core/PulseEngineBackend.h"

extern "C" __declspec(dllexport) IModule* CreateModule()
{
    return new MyCustomModule();
}

std::string MyCustomModule::GetName() const
{
    return std::string("CUSTOM MODULE");
}

std::string MyCustomModule::GetVersion() const
{
    return std::string("0.0.1");
}

void MyCustomModule::Initialize()
{
}

void MyCustomModule::Shutdown()
{
}

void MyCustomModule::Render()
{
    PulseInterfaceAPI::OpenWindow("Viewport");
    
    OpenGLAPI* openGLAPI = dynamic_cast<OpenGLAPI*>(PulseEngineInstance->graphicsAPI);
    if(openGLAPI)
        PulseInterfaceAPI::Image((int)openGLAPI->fboTexture, PulseEngine::Vector2(openGLAPI->fboWidth, openGLAPI->fboHeight), PulseEngine::Vector2(0, 1), PulseEngine::Vector2(1, 0));

        PulseInterfaceAPI::CloseWindow();
    
}
