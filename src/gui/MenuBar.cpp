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
  MenuBar::MenuBar(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window("MenuBar", p, s, win, f), checked(false) {

    }
    void MenuBar::Render() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if(ImGui::MenuItem("New")) {

                }
                if(ImGui::MenuItem("Open")) {
                    
                }
                if(ImGui::MenuItem("Save")) {
                    
                }
                ImGui::EndMenu();
            }
            
            // Cambia lo sfondo della voce Settings quando la finestra è aperta
            bool pushed_styles = false;
            if (bool_settings) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.0f, 0.9f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));
                pushed_styles = true;
            }
            
            if (ImGui::MenuItem("Settings")) {
                // Alterna lo stato della finestra settings
                bool_settings = !bool_settings;
            }
            
            // Ripristina lo stile originale se era stato modificato
            if (pushed_styles) {
                ImGui::PopStyleColor(4); // Pop all 4 style colors that were pushed
            }
            ImGui::SameLine();
            if (ImGui::MenuItem("Help")) {
                bool_HelpWindow = !bool_HelpWindow;
            }
            ImGui::EndMainMenuBar();
        }
    };
}