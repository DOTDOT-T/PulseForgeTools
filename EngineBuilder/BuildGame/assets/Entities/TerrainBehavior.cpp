#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/API/EngineApi.h"
#include "TerrainBehavior.h"

void TerrainBehavior::OnStart()
{
    BoxCollider* bx = owner->GetComponent<BoxCollider>();
    bx->mass = 9999.0f;
    bx->SetSize(PulseEngine::Vector3(80.0f,0.02f,80.0f));
}

void TerrainBehavior::OnUpdate()
{
    // Your update code here
}

void TerrainBehavior::OnRender()
{
    // Your render code here
}

void TerrainBehavior::OnEditorDisplay()
{
}

const char* TerrainBehavior::GetName() const
{
    return "PulseScriptTerrainBehavior";
}

// Register the script
extern "C" __declspec(dllexport) IScript* PulseScriptTerrainBehavior()
{
    return new TerrainBehavior();
}

