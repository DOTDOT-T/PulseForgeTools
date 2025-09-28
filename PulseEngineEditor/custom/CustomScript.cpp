#include "PulseEngine/CustomScripts/IScripts.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "CustomScript.h"
#include <iostream>

void CustomScript::OnStart()
{
}

void CustomScript::OnUpdate()
{
    parent->SetPosition(parent->GetPosition() + glm::vec3(0.001f * speed));
}

const char* CustomScript::GetName() const
{
    return "CreateCustomScriptA";
}



extern "C" __declspec(dllexport) IScript* CreateCustomScriptA()
{
    return new CustomScript();
}
