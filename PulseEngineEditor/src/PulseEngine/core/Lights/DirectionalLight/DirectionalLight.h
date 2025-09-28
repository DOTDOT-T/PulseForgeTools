#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Common/common.h"
#include "PulseEngine/core/Lights/Lights.h"
#include "dllExport.h"

class PulseEngineBackend;

class MA_DLL_API DirectionalLight : public LightData
{
    public:
        glm::vec3 direction;
        glm::mat4 lightSpaceMatrix;

        unsigned int depthMapFBO;
        unsigned int depthMapTex;

        float nearPlane;
        float farPlane;
        glm::vec3 target;

        DirectionalLight(float np, float fp, glm::vec3 t, glm::vec3 p, glm::vec3 c, float i, float a) : LightData(p, c, i, a), nearPlane(np), farPlane(fp), target(t)
        {
            glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, nearPlane, farPlane);
            glm::mat4 lightView = glm::lookAt(position, target, glm::vec3(0.0f, 0.0f, 1.0f));
            lightSpaceMatrix = lightProjection * lightView;

        }

        void InitShadowMap(int resolution);
        void RenderShadowMap(Shader &shader,PulseEngineBackend& scene) override;
        void BindToShader(Shader& shader, int index) override;
};

#endif