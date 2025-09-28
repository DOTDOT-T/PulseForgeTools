#ifndef EDITORDEFINES_H
#define EDITORDEFINES_H

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

#ifdef ENGINE_EDITOR

    // Helper function to get current date and time as a string
    inline std::string GetCurrentDateTime()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        std::tm* nowTm = std::localtime(&nowTime);

        std::ostringstream oss;
        oss << std::put_time(nowTm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    // Editor-specific macros
    #define EDITOR_ONLY(code) code
    #define IN_GAME_ONLY(code) // Nothing, stripped in editor builds
    #define EDITOR_LOG(msg) \
        std::cout << "[" << GetCurrentDateTime() << "] [Editor]: " << msg << std::endl;

#else

    // Game (runtime) specific macros
    #define EDITOR_ONLY(code) // Nothing, stripped in game builds
    #define IN_GAME_ONLY(code) code
    #define EDITOR_LOG(msg) // No editor log in game builds

#endif

#endif // EDITORDEFINES_H
