#include "PlayerBehavior.h"
#include "PulseEngine/API/MaterialAPI/MaterialApi.h"
#include "PulseEngine/core/Meshes/RenderableMesh.h"
#include "PulseEngine/API/GameEntity.h"
#include <algorithm>
#include <Windows.h>


extern "C" __declspec(dllexport) IScript* PulseScriptPlayerBehavior()
{
    return new PlayerBehavior();
}

void PlayerBehavior::OnStart()
{

    inputSystem = PulseEngineInstance->inputSystem;
    camera = PulseEngineInstance->GetActiveCamera();

    // camera initial placement relative to player
    camera->Position = PulseEngine::Vector3(0.0f, 6.0f, 10.0f);

    // cache meshes once
    wheelMeshes = owner->GetMeshesContainingName("wheel");
    turretMesh = owner->GetMeshContainingName("turret");
    cannonMesh = owner->GetMeshContainingName("cannon");
    chassisMesh = owner->GetMeshContainingName("chassis");

    // store respawn pos
    respawnPosition = owner->GetPosition();

    // initial health
    health = maxHealth;
    BoxCollider* bx = owner->GetComponent<BoxCollider>();

    bx->SetSize(PulseEngine::Vector3(4.0f, 2.0f, 6.0f));
    bx->mass = 250.0f;
    owner->SetPosition(PulseEngine::Vector3(0.0f,10.0f,0.0f));
}

void PlayerBehavior::OnUpdate()
{
    float dt = PulseEngineInstance->GetDeltaTime();

    // timers
    if (invulnerableTimer > 0.0f) invulnerableTimer = std::max(0.0f, invulnerableTimer - dt);
    if (fireTimer > 0.0f) fireTimer = std::max(0.0f, fireTimer - dt);

    UpdateLights(dt);
    HandleMovement(dt);
    HandleTurret(dt);
    HandleCamera(dt);
    HandleShooting(dt);
    BoxCollider* bx = owner->GetComponent<BoxCollider>();

    bx->physicBody = PhysicBody::MOVABLE;
}

void PlayerBehavior::UpdateLights(float dt)
{
    // déplacer le soleil avec le joueur (un seul directional light)
    for (auto* light : PulseEngineInstance->lights)
    {
        if (auto dir = dynamic_cast<DirectionalLight*>(light))
        {
            sunLight = dir;
            // offset ahead/right relative to player
            sunLight->SetPosition(owner->GetPosition() + PulseEngine::Vector3(5.0f, 5.0f, 0.0f));
            sunLight->target = owner->GetPosition();
        }
    }
}

void PlayerBehavior::HandleMovement(float dt)
{
    BoxCollider* bx =owner->GetComponent<BoxCollider>();

    if(inputSystem->isActionDown(0)) bx->velocity -= owner->GetTransform()->GetForward();
    if(inputSystem->isActionDown(1)) bx->velocity += owner->GetTransform()->GetForward();
    if(inputSystem->isActionDown(2)) owner->GetTransform()->AddWorldRotation(PulseEngine::Vector3(0.0f, 45.0f * dt, 0.0f));
    if(inputSystem->isActionDown(3)) owner->GetTransform()->AddWorldRotation(PulseEngine::Vector3(0.0f, -45.0f * dt, 0.0f));    

}

void PlayerBehavior::ApplyWheelRotation(float wheelSpeed, float dt)
{
    // rotate wheels on their local X by an amount proportional to travel
    if (wheelMeshes.empty())
    {
        wheelMeshes = owner->GetMeshesContainingName("wheel");
    }
    for (auto* mesh : wheelMeshes)
    {
        // Smooth rotation; sign based on forward/back keys
        float dir = 0.0f;
        if (inputSystem->isActionDown(0)) dir = -1.0f;
        if (inputSystem->isActionDown(1)) dir = 1.0f;
        mesh->transform.rotation.x += dir * wheelSpeed * dt;
    }
}

float PlayerBehavior::AngleDiff(float target, float current)
{
    float diff = target - current;
    while (diff > 180.0f) diff -= 360.0f;
    while (diff < -180.0f) diff += 360.0f;
    return diff;
}

void PlayerBehavior::HandleTurret(float dt)
{
    PulseEngine::Vector3 fwd = owner->GetTransform()->GetForward();

    float ownerYaw = atan2(fwd.x,
                         fwd.z) * 180.0f / 3.14159265f;

    float camYaw = atan2(PulseEngineInstance->GetActiveCamera()->Front.x,
                         PulseEngineInstance->GetActiveCamera()->Front.z) * 180.0f / 3.14159265f - 180.0f;

    turretMesh->transform.rotation.y = camYaw - ownerYaw;
    if (cannonMesh)
    {
        float camPitch = Clamp(camera->Pitch, -45.0f, 45.0f);
        cannonMesh->transform.rotation.x = camPitch + cannonPitchOffset;

        cannonMesh->transform.position.y = 0.94f;
    }
    
}

void PlayerBehavior::HandleCamera(float dt)
{
    // mouse look
    float mouseX = PulseEngineInstance->inputSystem->getMouseX();
    float mouseY = PulseEngineInstance->inputSystem->getMouseY();

    // center is hardcoded like before — consider moving to config
    const int centerX = 1920 / 2;
    const int centerY = 1080 / 2;

    camera->ProcessMouseMovement(mouseX - (float)centerX, (float)centerY - mouseY, true);
    SetCursorPos(centerX, centerY);

    // follow player with offset
    camera->Position = owner->GetPosition() - camera->Front * 10.0f + PulseEngine::Vector3(0.0f, 2.5f, 0.0f);
}

void PlayerBehavior::HandleShooting(float dt)
{
    // left mouse or action mapping => shoot
    bool shoot = inputSystem->isActionDown(4) || PulseEngineInstance->inputSystem->wasActionPressed(4); // assuming mouse button API

    if (shoot && fireTimer <= 0.0f && ammo > 0)
    {
        // spawn projectile in front of cannon
        if (cannonMesh)
        {
            PulseEngine::Vector3 spawnPos = cannonMesh->transform.GetWorldPosition() + cannonMesh->transform.GetForward() * -5.0f;
            Entity* proj = GameEntity::Instantiate("Entities/Cube.pEntity", spawnPos);
            proj->collider->mass = 50.0f;
            proj->collider->physicBody = PhysicBody::MOVABLE;
            proj->collider->SetSize(PulseEngine::Vector3(0.4f,0.4f,0.4f));
            proj->collider->velocity = cannonMesh->transform.GetForward() * -100.0f;

            owner->GetComponent<BoxCollider>()->force += cannonMesh->transform.GetForward() * 5000.0f;

            ammo--;
            fireTimer = fireCooldown;
        }
    }

    // simple reload when empty (manual or auto)
    if (ammo <= 0 && fireTimer <= 0.0f)
    {
        // auto-reload with delay (example)
        ammo = 20;
        fireTimer = 1.0f;
    }
}

void PlayerBehavior::OnRender()
{
    // debug rendering possible here (health, ammo, cooldown)
    // TODO: use engine debug draw to show HUD elements if available.
}

void PlayerBehavior::OnEditorDisplay()
{
}

const char* PlayerBehavior::GetName() const
{
    return "PulseScriptPlayerBehavior";
}
