#include "InputBinding.h"
#include "PulseEngineEditor/InterfaceEditor/InterfaceAPI/PulseInterfaceAPI.h"
#include "PulseEngine/core/Input/InputSystem.h"

std::string InputBinding::GetName() const
{
    return std::string("Input Binding");
}

std::string InputBinding::GetVersion() const
{
    return std::string("v0.1.0");
}

void InputBinding::Initialize()
{
    inputSystem = PulseEngineInstance->inputSystem;
}

void InputBinding::Shutdown()
{
}

char TurnIntToChar(int i)
{
    if(i >= 0 && i < 255) return static_cast<char>(i);

    return '?';
}

void InputBinding::Render()
{
    PulseInterfaceAPI::OpenWindow("Input Binding###InputBinding");
    PulseInterfaceAPI::WriteText("All inputs available :");
    PulseInterfaceAPI::WriteText("Action ID\tKey\tMouse");
    static bool modifyBinding = false;
    static int bindingToModify = -1;
    if (inputSystem)
    {

        PulseInterfaceAPI::OpenTable("InputBindingsTable", 3);
        PulseInterfaceAPI::DeclareTableColumn("Action ID");
        PulseInterfaceAPI::DeclareTableColumn("Key");
        PulseInterfaceAPI::DeclareTableColumn("Mouse");
        PulseInterfaceAPI::DrawTableHeadersRow();
        for(unsigned int i = 0; i < 512; i++)
        {
            PulseInterfaceAPI::Separator();
            OneBinding bind = inputSystem->GetOneBinding(i);

            

            std::string actionStr = std::to_string(i) + "\t";
            std::string keyStr = (bind.actionId != -1) ? std::string(1, TurnIntToChar(bind.actionId)) : "-";
            std::string mouseStr = (bind.button != -1) ? std::string(1, TurnIntToChar(bind.button)) : "-";

            PulseInterfaceAPI::NextTableColumn();
            PulseInterfaceAPI::SetTableColumnIndex(0);
            if(PulseInterfaceAPI::Button(actionStr, PulseEngine::Vector2(0.0f,0.0f)))
            {
                modifyBinding = !modifyBinding;
                bindingToModify = i;
            }
            PulseInterfaceAPI::SetTableColumnIndex(1);
            PulseInterfaceAPI::WriteText(keyStr);
            PulseInterfaceAPI::SetTableColumnIndex(2);
            PulseInterfaceAPI::WriteText(mouseStr);            
        }

        PulseInterfaceAPI::EndTable();

        if(modifyBinding && bindingToModify != -1)
        {
            
                // Open a new window to modify the binding
                PulseInterfaceAPI::OpenWindow("Modify Binding###ModifyBinding");
                PulseInterfaceAPI::WriteText("Modify binding for Action " + std::to_string(bindingToModify));
                OneBinding newBind = inputSystem->GetOneBinding(bindingToModify);
                if(PulseInterfaceAPI::BeginCombo("Key", (newBind.actionId != -1) ? std::string(1, TurnIntToChar(newBind.actionId)).c_str() : "-"))
                {
                    for(int i = 32; i < 96; i++)
                    {
                        if(PulseInterfaceAPI::Selectable(std::string(1, TurnIntToChar(i)), newBind.actionId == i))
                        {
                            newBind.actionId = i;
                        }

                        inputSystem->ChangeBinding(newBind, bindingToModify);
                        inputSystem->SaveBindingsToFile();
                    }
                    PulseInterfaceAPI::EndCombo();
                }
                PulseInterfaceAPI::CloseWindow();
        }
    }

    PulseInterfaceAPI::CloseWindow();
}


extern "C" __declspec(dllexport) IModule* CreateModule()
{
    return new InputBinding();
}