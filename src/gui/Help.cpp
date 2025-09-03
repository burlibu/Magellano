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
  HelpWindow::HelpWindow(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, pos, size, win, f) {
    }

    void HelpWindow::Render() {
        // Aggiorna le dimensioni della finestra in base al frame principale
        int current_width, current_height;
        glfwGetFramebufferSize(window_ptr, &current_width, &current_height);

        // Calcola la posizione e dimensione in base al frame attuale
        ImVec2 current_pos = ImVec2(0, menubar_height);
        ImVec2 current_size = ImVec2(current_width, current_height - menubar_height);
        
        // Imposta la posizione e dimensione aggiornate
        ImGui::SetNextWindowPos(current_pos);
        ImGui::SetNextWindowSize(current_size);

        if (ImGui::Begin(title.c_str(), NULL, flags_HelpWindow)) {
            // Apply custom style for settings window
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 6));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
            
            // Create tabs for different setting categories
            if (ImGui::BeginTabBar("SettingsTabs", ImGuiTabBarFlags_None)) {
                // General Tab
                if (ImGui::BeginTabItem("Tutorial")) {
                    
                    
                    ImGui::EndTabItem();
                }
                
                // Display Tab
                if (ImGui::BeginTabItem("Fix Bugs")) {
                    
                    
                    ImGui::EndTabItem();
                }
                
                // Audio Tab
                if (ImGui::BeginTabItem("Contact")) {
                    
                    
                    ImGui::EndTabItem();
                }
                
                // Advanced Tab
                
                ImGui::EndTabBar();
            }
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            
            // Action buttons at the bottom
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 360);
            if (ImGui::Button("Reset to Defaults", ImVec2(150, 30))) {
                ImGui::OpenPopup("Reset Settings");
            }
            
            ImGui::SameLine();
            
            ImGui::PopStyleVar(2); // Pop the style variables we pushed earlier
        }

        ImGui::End();

    }
}