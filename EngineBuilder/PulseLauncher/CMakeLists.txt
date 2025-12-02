cmake_minimum_required(VERSION 3.21)
project(PulseLauncherProject)

set(CMAKE_CXX_STANDARD 20)

# GLFW
add_subdirectory(external/glfw)

# ImGui sources
set(IMGUI_SRC
    include/imgui/imgui.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/ImGuizmo.cpp

    include/imgui/backends/imgui_impl_glfw.cpp
    include/imgui/backends/imgui_impl_opengl3.cpp
)

# exe
add_executable(PulseLauncher
    src/main.cpp
    src/PulseLauncher.cpp
    src/glad.c
    ${IMGUI_SRC}
)

target_include_directories(PulseLauncher PRIVATE
    include
    include/imgui
    include/glad
    external/glfw/include

)

target_link_libraries(PulseLauncher PRIVATE glfw opengl32)
