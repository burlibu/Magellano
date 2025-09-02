//standards
#include <iostream>
#include <thread>
#include <vector>
//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
//src
#include "../env.h"
#include "../settings.h"
#include "../functions.h"
#include "../custom_colors.h"
#include "gui.h"

namespace Gui {
  showDemoWindow::showDemoWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f) 
    : Window("Demo Window", p, s, win, f){ // costruttore con lista di inizializzazione

    };
    void showDemoWindow::Render() {
        ImGui::ShowDemoWindow(&bool_demo_window);
    };
}