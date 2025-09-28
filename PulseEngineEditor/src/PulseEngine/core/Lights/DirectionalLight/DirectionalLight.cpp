#include "DirectionalLight.h"
#include "PulseEngine/core/Entity/Entity.h"

void DirectionalLight::InitShadowMap(int resolution)
{
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMapTex);
    glBindTexture(GL_TEXTURE_2D, depthMapTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalLight::RenderShadowMap(Shader &shader, PulseEngineBackend &scene)
{
    glViewport(0, 0, 2048, 2048);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT); 


    for (Entity* obj : scene.entities)
    {
        shader.Use();
        shader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
        shader.SetMat4("model", obj->GetMatrix());
        obj->SimplyDrawMesh(); 
    }

    glCullFace(GL_BACK); 
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalLight::BindToShader(Shader &shader, int index)
{
    shader.SetVec3("lightPositions[" + std::to_string(index) + "]", position);
    shader.SetVec3("lightColors[" + std::to_string(index) + "]", color);
    shader.SetFloat("lightIntensities[" + std::to_string(index) + "]", intensity);
    shader.SetFloat("lightAttenuations[" + std::to_string(index) + "]", attenuation);
    shader.SetInt("lightTypes[" + std::to_string(index) + "]", 0); // 0 = directional
}
