#ifndef INPUTBINDING_H
#define INPUTBINDING_H

#include "Common/common.h"
#include "Common/dllExport.h"

#include  "PulseEngine/ModuleLoader/IModuleInterface/IModuleInterface.h"

class Mesh;

class InputBinding : public IModuleInterface
{
    public:
        std::string GetName() const override;
        std::string GetVersion() const override;
        void Initialize() override;
        void Shutdown() override;
        void Render() override;

        InputSystem* inputSystem = nullptr;
};

#endif