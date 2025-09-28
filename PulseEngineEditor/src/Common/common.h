#ifndef COMMON_H
#define COMMON_H

#include <glad.h>
#include <GLFW/glfw3.h>

#include "Common/EditorDefines.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "camera.h"
#include "shader.h"
#include "PulseEngine/core/WindowContext/WindowContext.h"




#ifdef ENGINE_EDITOR
    #include "imgui.h"
    #include "backends/imgui_impl_glfw.h"
    #include "backends/imgui_impl_opengl3.h"
#endif

#ifdef ENGINE_EDITOR
#define ASSET_PATH "PulseEngineEditor/"
#endif
#ifndef ENGINE_EDITOR
#define ASSET_PATH "assets/"
#endif

using GUID = std::size_t;
#endif