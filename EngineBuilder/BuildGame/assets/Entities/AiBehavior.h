#pragma once

#include "IScripts.h"
#include "Common/dllExport.h"
#include "Engine.h"

class PULSE_ENGINE_DLL_API AiBehavior : public IScript
{
    public:
        AiBehavior()
        {
            // if you need to expose variables, do it here with these macro : 
            // AddExposedVariable(EXPOSE_VAR(speed, INT));
            // REGISTER_VAR(speed); both are needed to make the variable exposed in the editor
        }
        void OnStart() override;
        void OnUpdate() override;
        void OnRender() override;
        void OnEditorDisplay() override;
        const char* GetName() const override;

        PulseEngine::Vector3 target;
        Shader* lineShader;
};

