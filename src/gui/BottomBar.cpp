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
  BottomBar::BottomBar(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, pos, size, win, f) {
        
    }

    void BottomBar::Render() {
        int frame_current_width, frame_current_height;
        glfwGetFramebufferSize(window_ptr, &frame_current_width, &frame_current_height);
        pos = ImVec2(0, frame_current_height - height);
        size = ImVec2(frame_current_width, height);
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, blu_oscuro);
        ImGui::Begin(title.c_str(), &bool_BottomBar , flags_BottomBar);
        ImGui::PushStyleColor(ImGuiCol_Text, bianco);
        std::string status_text = "version: " + version + " | resolution: "
          + std::to_string(frame_current_width) + "x" + std::to_string(frame_current_height);
        const ImVec2 status_text_size = ImGui::CalcTextSize(status_text.c_str());
        ImGui::SetCursorPos(ImVec2(size.x - status_text_size.x - 12.0f, size.y - 25.0f));
        ImGui::TextUnformatted(status_text.c_str());
        ImGui::PopStyleColor();
        ImGui::End();
        ImGui::PopStyleColor();
    }
}