#ifndef LIGHTS_H
#define LIGHTS_H

#include "Common/common.h"
#include "dllExport.h"

class PulseEngineBackend;

class MA_DLL_API LightData
{
    public:
        glm::vec3 position;
        glm::vec3 color;
        float intensity;
        float attenuation;
        bool castsShadow;
    
        virtual void BindToShader(Shader& shader, int index) = 0;
        virtual void RenderShadowMap(Shader &shader, PulseEngineBackend& scene) = 0;
    
        LightData(glm::vec3 p, glm::vec3 c, float i, float a) : position(p), color(c), intensity(i), attenuation(a), castsShadow(true) {}
};

#endif
