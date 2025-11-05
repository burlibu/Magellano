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
#include "../settings/env.h"
#include "../settings/settings.h"
#include "../lib/functions.h"
#include "../lib/custom_colors.h"
#include "gui.h"

namespace Gui {
  Window1::Window1(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window("Window1", p, s, win, f) {

    }

    void Window1::Render() {
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::Begin("Window1", &bool_window1, flags_window1);

    if (ImGui::Button("My button", ImVec2(100,100))) {
        // tutto quello in questo if verrà eseguito quando il bottone sarà premuto
        ImGui::Text("You pressed the button");
    }
    bool checkBoxValue = true;
    ImGui::Checkbox("CheckBox", &checkBoxValue);

    ImGui::SliderFloat("My Float Slider", &value1, 0.0f, 100.0f);

    ImGui::Combo("##id1", &current, items.data(), items.size());
    ImGui::SetCursorPosX(ImGui::GetWindowWidth()/2 - ImGui::CalcTextSize("This is some text").x/2);
    ImGui::Text("This is some text");
    ImGui::End();
    };
}