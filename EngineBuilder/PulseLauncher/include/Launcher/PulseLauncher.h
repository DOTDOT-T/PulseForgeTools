#pragma once
#include "imgui/imgui.h"
#include <glad/glad.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <chrono>

class PulseLauncher
{
public:
    PulseLauncher();
    ~PulseLauncher();

    void UpdateLauncher();
    void RenderLauncher();
    void CleanupLauncher();
    GLFWwindow* GetWindow() const;

    // New : load project metadata
    void StartLaunchCountdown(const std::string& projectName,
                              const std::string& engineExe,
                              const std::string& projectDir);

private:
    GLFWwindow* window = nullptr;

    // Splash mode
    bool launching = false;
    std::chrono::steady_clock::time_point launchStartTime;
    std::string projectName;
    std::string enginePath;
    std::string projectPath;
};
