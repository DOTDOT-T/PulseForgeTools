#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/API/EngineApi.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/API/EntityApi/EntityApi.h"
#include "cust.h"

void cust::OnStart()
{
    input.bindAction("jump", ' ');
}

void cust::OnUpdate()
{
    if(input.wasActionPressed("jump"))
    {
        //jump logic of the player
    }
    
}

void cust::OnRender()
{
    // Your update code here
}

const char* cust::GetName() const
{
    return "PulseScriptFROMPULSE";
}

// Register the script
extern "C" __declspec(dllexport) IScript* PulseScriptFROMPULSE()
{
    return new cust();
}

