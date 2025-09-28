#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include "Common/common.h"
#include "dllExport.h"
class LightData;
class DirectionalLight;

class MA_DLL_API LightManager
{
    public:
        std::vector<DirectionalLight*> directionalLights;
        void RenderAllShadowsMap(PulseEngineBackend& scene);

};


#endif