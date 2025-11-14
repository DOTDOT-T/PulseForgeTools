#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/API/EngineApi.h"
#include "PulseEngine/API/MaterialAPI/MaterialApi.h"
#include "PulseEngine/core/Physics/Cast/Casting.h"
#include "AiBehavior.h"

void AiBehavior::OnStart()
{
    target = PulseEngine::Vector3(0.0f);
    lineShader = new Shader("PulseEngineEditor/shaders/lineTrace.vert","PulseEngineEditor/shaders/lineTrace.frag", PulseEngineGraphicsAPI);
}

void AiBehavior::OnUpdate()
{
    // Your update code here
}

void AiBehavior::OnRender()
{
    // Your render code here
}

void AiBehavior::OnEditorDisplay()
{

    PulseEngine::Physics::CastData cd;
    cd.start = owner->GetPosition() - PulseEngine::Vector3(0.0f, 2.0f, 0.0f);
    cd.end = owner->GetPosition() - PulseEngine::Vector3(0.0f, 10.0f, 0.0f);
    cd.step = 0.01f;

    PulseEngine::Physics::CastResult rs = PulseEngine::Physics::Casting::Cast(cd);    
    target = rs.hitEntity ? rs.impactLocation : PulseEngine::Vector3(0.0f, -10.0f, 0.0f);

    lineShader->Use();
    lineShader->SetMat4("view", PulseEngineInstance->lastView);
    lineShader->SetMat4("projection", PulseEngineInstance->lastProjection);
    lineShader->SetMat4("model", PulseEngine::MathUtils::Matrix::Identity()); 
    lineShader->SetVec3("color", PulseEngine::Vector3(1.0f, 0.0f, 0.0f));
    PulseEngineGraphicsAPI->ActivateWireframe();
    PulseEngineGraphicsAPI->DrawLine(owner->GetPosition() - PulseEngine::Vector3(0.0f, 2.0f, 0.0f), target, PulseEngine::Color(1.0f, 0.0f, 0.0f));
    lineShader->SetVec3("color", PulseEngine::Vector3(0.0f, 1.0f, 0.0f));
    PulseEngineGraphicsAPI->DrawLine(owner->GetPosition(), owner->GetPosition() - PulseEngine::Vector3(0.0f, 10.0f, 0.0f), PulseEngine::Color(0.0f, 1.0f, 0.0f));
    PulseEngineGraphicsAPI->DesactivateWireframe();
}

const char* AiBehavior::GetName() const
{
    return "PulseScriptAiBehavior";
}

// Register the script
extern "C" __declspec(dllexport) IScript* PulseScriptAiBehavior()
{
    return new AiBehavior();
}

