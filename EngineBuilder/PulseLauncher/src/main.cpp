#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>

#include "Launcher/PulseLauncher.h"
#include "json.hpp"

#include <string>
#include <windows.h>

int main(int argc, char** argv)
{
    PulseLauncher launcher;
    if (argc > 1)
    {

        std::string projectPath = argv[1];

        std::ifstream file(projectPath);
        nlohmann::json js;
        file >> js;

        std::string engineExe = js["enginePath"];
        std::string projectDir = js["projectDir"];
        launcher.StartLaunchCountdown(
            js["enginePath"],
            js["projectDir"]
        );
    }


    // Main loop
    while (!glfwWindowShouldClose(launcher.GetWindow()))
    {
        launcher.UpdateLauncher();
        launcher.RenderLauncher();
    }

    return 0;
}


