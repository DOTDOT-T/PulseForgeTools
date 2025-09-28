#include <filesystem>
#include <iostream>
#include <string>

int main()
{
    std::string output("outputs/Module.dll");
    std::cout << "Compiling...\n";
                    //Lets work on the custom files scripts now
                std::string compiler = "g++";
                std::string stdVersion = "-std=c++17";
                std::string defines = "-DBUILDING_DLL -DWINDOW_PULSE_EXPORT dist/src/glad.c";
                std::string flags = "-shared -Wall -g -mconsole " + defines;
                std::string includeDirs = R"(-IUserScripts -Idist\include -Idist\src -Idist -Ldist)";
                std::string libs = "-lPulseEngine -lglfw3 -lgdi32 -lopengl32";

                // Gather source files
                std::string sources;
                for (const auto& entry : std::filesystem::directory_iterator("UserScripts"))
                {
                    if (entry.path().extension() == ".cpp")
                    {
                        sources += std::filesystem::path(entry.path()).generic_string() + " ";
                    }
                }
            
                // Final compilation command
                std::string compileCommand = compiler + " " + stdVersion +
                                             " -o " + output + " " + sources +
                                             includeDirs + " " + libs + " " + flags;
            
                std::cout << "Compiling with command:\n" << compileCommand << "\n";
            
                int result = system(compileCommand.c_str());
                if (result != 0)
                {
                    std::cerr << "Compilation failed.\n";
                }
                else
                {
                    std::cout << "DLL generated: " << output << "\n";
                }

    while(true) {}
    return 0;
}