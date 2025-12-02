#include "Launcher/PulseLauncher.h"
#include <imgui/imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <cstdio>
#include <string>
#include <windows.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

std::wstring ToWide(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(),
                                          (int)str.size(), nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(),
                        (int)str.size(), &wstr[0], size_needed);
    return wstr;
}


bool LaunchEngineProcess(const std::string& engineExe,
                         const std::string& projectDir)
{
    std::wstring wEngine = ToWide(engineExe);
    std::wstring wArgs = L"\"" + ToWide(engineExe) + L"\" \"" + ToWide(projectDir) + L"\"";

    STARTUPINFOW si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);

    BOOL ok = CreateProcessW(
        wEngine.c_str(),     // Application
        &wArgs[0],           // Arguments (modifiable buffer)
        nullptr,
        nullptr,
        FALSE,
        CREATE_NEW_CONSOLE,  // ou 0 pour silencieux
        nullptr,
        nullptr,
        &si,
        &pi
    );

    if (!ok)
    {
        DWORD err = GetLastError();
        printf("CreateProcessW failed: %lu\n", err);
        return false;
    }

    // On peut fermer les handles (l'engine vit tout seul)
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}


PulseLauncher::PulseLauncher()
    : window(nullptr)
{
    // GLFW init
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return;

    // Window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "Pulse Launcher", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // V-Sync

    // Load OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr;
        return;
    }

    // ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Dark theme style
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.ChildRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.50f, 0.80f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.60f, 0.95f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.45f, 0.75f, 1.00f);
}

PulseLauncher::~PulseLauncher()
{
    CleanupLauncher();
}

GLFWwindow* PulseLauncher::GetWindow() const
{
    return window;
}

void PulseLauncher::UpdateLauncher()
{
    if (window)
        glfwPollEvents();
}

void PulseLauncher::RenderLauncher()
{
    if (!window) return;

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // --- Splash screen countdown ---
    if (launching)
    {
        float elapsed = std::chrono::duration<float>(
            std::chrono::steady_clock::now() - launchStartTime
        ).count();

        ImGui::SetNextWindowSize(ImVec2(display_w, display_h));
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoInputs;

        ImGui::Begin("Splash", nullptr, flags);

        ImGui::SetCursorPos(ImVec2(display_w * 0.35f, display_h * 0.40f));
        ImGui::Text("Loading Project...");
        
        ImGui::SetCursorPos(ImVec2(display_w * 0.35f, display_h * 0.48f));
        ImGui::Text("%s", projectName.c_str());

        // Fake progress bar
        float progress = elapsed / 3.0f;
        if (progress > 1.0f) progress = 1.0f;

        ImGui::SetCursorPos(ImVec2(display_w * 0.25f, display_h * 0.60f));
        ImGui::ProgressBar(progress, ImVec2(display_w * 0.50f, 30));

        ImGui::End();

        // After 3 seconds → launch engine
        if (elapsed >= 3.0f)
        {
            LaunchEngineProcess(enginePath, projectPath);
            glfwSetWindowShouldClose(window, true);
        }
    }
    else
    {
        // --- Normal launcher view ---
        ImGui::Begin("Pulse Launcher");
        ImGui::Text("Waiting for project...");
        ImGui::End();
    }

    // Render frame
    ImGui::Render();
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(window);
    }

    glfwSwapBuffers(window);
}

void PulseLauncher::CleanupLauncher()
{
    if (!window) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    window = nullptr;
}
void PulseLauncher::StartLaunchCountdown(const std::string& projectName_,
                                         const std::string& engineExe_,
                                         const std::string& projectDir_)
{
    projectName = projectName_;
    enginePath = engineExe_;
    projectPath = projectDir_;

    launchStartTime = std::chrono::steady_clock::now();
    launching = true;
}
