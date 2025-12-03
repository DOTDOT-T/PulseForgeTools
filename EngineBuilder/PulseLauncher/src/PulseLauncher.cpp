#include "Launcher/PulseLauncher.h"

PulseLauncher::PulseLauncher()
{
    // Hide console
    HWND hwndConsole = GetConsoleWindow();
    if (hwndConsole)
        ShowWindow(hwndConsole, SW_HIDE);

    // GLFW init
    glfwSetErrorCallback([](int error, const char* desc){ fprintf(stderr, "GLFW Error %d: %s\n", error, desc); });
    if (!glfwInit()) return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    int win_w = 1200;
    int win_h = 600;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    win_x = (mode->width - win_w) / 2;
    win_y = (mode->height - win_h) / 2;

    window = glfwCreateWindow(win_w, win_h, "Pulse Launcher", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    glfwSetWindowPos(window, win_x, win_y);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr;
        return;
    }

    // ImGui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Dark theme
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f,0.08f,0.09f,1.0f);
    style.Colors[ImGuiCol_Text] = ImVec4(0.95f,0.96f,0.98f,1.0f);
}

PulseLauncher::~PulseLauncher()
{
    CleanupLauncher();
}

GLFWwindow* PulseLauncher::GetWindow() const { return window; }

void PulseLauncher::StartLaunchCountdown(const std::string& engineExe, const std::string& projectDir)
{
    launching = true;
    std::thread([this, engineExe, projectDir]() { LaunchEngine(engineExe, projectDir); }).detach();
}

void PulseLauncher::LaunchEngine(const std::string& engineExe, const std::string& projectDir)
{
    SECURITY_ATTRIBUTES sa{ sizeof(sa), nullptr, TRUE };
    CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
    SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOW si{};
    si.cb = sizeof(si);
    si.hStdOutput = hWritePipe;
    si.hStdError  = hWritePipe;
    si.dwFlags |= STARTF_USESTDHANDLES;

    std::wstring wExe(engineExe.begin(), engineExe.end());
    std::wstring wArgs = L"\"" + wExe + L"\" \"" + std::wstring(projectDir.begin(), projectDir.end()) + L"\"";

    if (!CreateProcessW(
        wExe.c_str(),
        &wArgs[0],
        nullptr, nullptr,
        TRUE,
        CREATE_NEW_CONSOLE,
        nullptr, nullptr,
        &si,
        &pi))
    {
        DWORD err = GetLastError();
        std::lock_guard<std::mutex> lock(mtx);
        messages.push("Failed to launch engine: " + std::to_string(err));
        return;
    }

    CloseHandle(hWritePipe);
    
    // Poll engine stdout
    PollEngineMessages();
}

void PulseLauncher::PollEngineMessages()
{
    char buffer[256];
    DWORD bytesRead = 0;
    std::string line;

    while (true)
    {
        if (!ReadFile(hReadPipe, buffer, sizeof(buffer)-1, &bytesRead, nullptr) || bytesRead == 0)
            break;

        buffer[bytesRead] = 0;
        line += buffer;

        size_t pos;
        while ((pos = line.find('\n')) != std::string::npos)
        {
            std::string msg = line.substr(0,pos);
            line.erase(0,pos+1);

            std::lock_guard<std::mutex> lock(mtx);
            messages.push(msg);

            if (msg.find("Finished the initialization of the engine.") != std::string::npos)
            {
                // glfwSetWindowShouldClose(window, true);
                progressValue = 1.0;
                return;
            }
        }
    }
}

void PulseLauncher::UpdateLauncher()
{
    if (window)
        glfwPollEvents();

    std::lock_guard<std::mutex> lock(mtx);
    while (!messages.empty())
    {
        std::string msg = messages.front();
        messages.pop();

        statusText = msg;
        logMessages.push_back(msg);

        if (logMessages.size() > 5000)
            logMessages.pop_front();
    }
}

void PulseLauncher::RenderLauncher()
{
    if (!window) return;

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    LoadingWindow(w * 0.25f, h);

    ImGui::SetNextWindowSize(ImVec2((float)w * 0.75f, (float)h));
    ImGui::SetNextWindowPos(ImVec2((float)w * 0.25f, 0));
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar;
    
    ImGui::Begin("Console Log", nullptr, flags);
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
    
    for (const auto& line : logMessages)
    {
        ImVec4 color = line.find("[ERROR]") != std::string::npos ? ImVec4(1.0,0.0,0.0,1.0) : line.find("[INFO]") != std::string::npos ? ImVec4(0.0, 0.5, 1.0, 1.0) : line.find("[WARN]") != std::string::npos ? ImVec4(1.0,1.0,0.0,1.0) : ImVec4(1.0,1.0,1.0, 1.0);
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(line.c_str());
        ImGui::PopStyleColor();
    }
    
    if (autoScroll)
        ImGui::SetScrollHereY(1.0f);
    
    ImGui::EndChild();
    ImGui::End();

    ImGui::Render();
    glViewport(0,0,w,h);
    glClearColor(0.12f,0.12f,0.12f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void PulseLauncher::LoadingWindow(int w, int h)
{
    ImGui::SetNextWindowSize(ImVec2((float)w, (float)h));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs;
    ImGui::Begin("Splash", nullptr, flags);

    ImVec2 textSize = ImGui::CalcTextSize(progressValue >= 1.0f ? "Engine loaded." : statusText.c_str());
    ImGui::SetCursorPos(ImVec2((w - textSize.x) * 0.5f, h * 0.45f));
    ImGui::Text("%s", progressValue >= 1.0f ? "Engine loaded." : statusText.c_str());

    ImGui::SetCursorPos(ImVec2(w * 0.25f, h * 0.55f));
    ImGui::ProgressBar(progressValue, ImVec2(w * 0.5f, 30));

    ImGui::End();
}
void PulseLauncher::CleanupLauncher()
{
    if (!window) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (pi.hProcess) CloseHandle(pi.hProcess);
    if (pi.hThread) CloseHandle(pi.hThread);
    if (hReadPipe) CloseHandle(hReadPipe);

    glfwDestroyWindow(window);
    glfwTerminate();
    window = nullptr;
}
