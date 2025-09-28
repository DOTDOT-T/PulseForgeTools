#ifndef MOUSEINPUT_H
#define MOUSEINPUT_H

#include "Common/common.h"
#include "dllExport.h"

class MA_DLL_API MouseInput
{
    public:
        static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif