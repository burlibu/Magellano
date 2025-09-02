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
  AttackWindow::AttackWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window("Attack Window", p, s, win, f) {

    }
    void AttackWindow::Render() {
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::Begin("Attack", &bool_esperimenti_window, flags_esperimenti_window);

        // Input text
        static char text[128] = "";
        ImGui::InputText("Target Ip", text, IM_ARRAYSIZE(text));

        // Checkbox
        static bool malware_checkbox = false;
        ImGui::Checkbox("Deploy Malware", &malware_checkbox);

        static bool spread_checkbox = false;
        ImGui::Checkbox("Spread to nearby devices", &spread_checkbox);

        // Radio buttons
        static int radio = 0;
        ImGui::RadioButton("Ddos Attack : deny connection and services to and from Rtarget by flooding it with junk traffic.", &radio, 0);
        ImGui::RadioButton("Reverse Shell : obtain access to the target operating system in order to gather information or modify it.", &radio, 1);
        ImGui::RadioButton("Malware injection : insert in the target's device a malicious code capable of incapacitating defenses or even destroy it.", &radio, 2);
        

        // Color picker
        static float color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
        ImGui::ColorEdit4("Colore", color);

        ImGui::End();
    }
}