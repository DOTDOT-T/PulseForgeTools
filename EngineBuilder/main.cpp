#include "PulseEngine/core/PulseObject/PulseObject.h"
#include "PulseEngine/core/GUID/GuidGenerator.h"
#include "PulseEngine/core/FileManager/FileManager.h"
#include "PulseEngine/core/FileManager/FileReader/FileReader.h"

#include <iostream>
#include <windows.h>



int main()
{
    std::cout << PulseEngine::Registry::GenerateGUIDFromPath("helloteam") << std::endl;
    std::cout << PulseEngine::Registry::GenerateGUIDFromPathAndMap("helloteam", "map_01") << std::endl;

    PulseEngine::FileSystem::FileReader config = PulseEngine::FileSystem::FileReader("Config.json");

    config.SaveJson(PulseEngine::FileSystem::FileManager::OpenEngineConfigFile());

    std::cin.get();

    return 0;
}