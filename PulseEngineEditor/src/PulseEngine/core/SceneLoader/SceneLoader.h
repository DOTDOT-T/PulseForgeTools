#ifndef SCENELOADER_H
#define SCENELOADER_H

#include "Common/common.h"
#include "dllExport.h"

class PulseEngineBackend;

class MA_DLL_API SceneLoader
{
    public:
        static void LoadScene(const std::string& mapName, PulseEngineBackend* backend);
        static std::vector<std::string> GetSceneFiles(const std::string& directory);
        static void SaveSceneToFile(const std::string& mapName, PulseEngineBackend* backend);
    private:
};

#endif