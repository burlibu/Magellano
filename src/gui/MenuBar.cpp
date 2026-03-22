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

using namespace std;
using namespace ImGui;

namespace Gui {
  MenuBar::MenuBar(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window("MenuBar", p, s, win, f), checked(false) {

    }
    void MenuBar::Render() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if(ImGui::MenuItem("New")) {
                    //TODO
                }
                if(ImGui::MenuItem("Open")) {
                    //TODO
                }
                if(ImGui::MenuItem("Save")) {
                    // TODO
                }
                ImGui::EndMenu();
            }

            bool first_top_panel_item = true;
            for (const TopPanelToggleEntry& entry : top_panel_toggle_entries) {
                if (!first_top_panel_item) {
                    ImGui::SameLine();
                }
                first_top_panel_item = false;

                bool pushed_styles = false;
                if (entry.state != nullptr && *(entry.state)) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Header, entry.active_header_color);
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, entry.hovered_header_color);
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, entry.pressed_header_color);
                    pushed_styles = true;
                }

                if (ImGui::MenuItem(entry.window_name)) {
                    toggleExclusiveTopPanel(entry.state);
                }

                if (pushed_styles) {
                    ImGui::PopStyleColor(4);
                }
            }
            ImGui::EndMainMenuBar();
        }
    };
}