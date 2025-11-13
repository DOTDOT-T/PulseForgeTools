#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/API/EngineApi.h"
#include "PulseEngine/API/GameEntity.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "CasualEnemy.h"

void CasualEnemy::OnStart()
{
    targetEntity = GameEntity::GetAllEntitiesContainingName("o.")[0];
    physicComponent = owner->GetComponent<BoxCollider>();

    physicComponent->SetSize(PulseEngine::Vector3(4.0f, 2.0f, 6.0f));
    physicComponent->mass = 250.0f;

}

void CasualEnemy::OnUpdate()
{
    physicComponent->velocity += (targetEntity->GetPosition() - owner->GetPosition()).Normalized() * 10.0f;
    physicComponent->physicBody = PhysicBody::MOVABLE;
}

void CasualEnemy::OnRender()
{
    // Your render code here
}

void CasualEnemy::OnEditorDisplay()
{
}

const char* CasualEnemy::GetName() const
{
    return "PulseScriptCasualEnemy";
}

// Register the script
extern "C" __declspec(dllexport) IScript* PulseScriptCasualEnemy()
{
    return new CasualEnemy();
}

