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
  TabWindow::TabWindow(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, pos, size, win, f) {}

    void TabWindow::Render(){
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        if (ImGui::Begin(title.c_str(), nullptr, flags_tab_window)) {
            if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_Reorderable)) {
                // Tab fisso (non chiudibile)
                if (ImGui::BeginTabItem("Generale")) {
                    ImGui::Text("Contenuto tab generale");
                    if (ImGui::Button("Azione 1")) {
                        // Azione del bottone
                    }
                    ImGui::EndTabItem();
                }

                // Tab chiudibili
                if (tab1_open && ImGui::BeginTabItem("Tab 1", &tab1_open)) {
                    ImGui::Text("Contenuto tab 1");
                    ImGui::EndTabItem();
                }

                if (tab2_open && ImGui::BeginTabItem("Tab 2", &tab2_open, ImGuiTabItemFlags_UnsavedDocument)) {
                    ImGui::Text("Contenuto tab 2 (non salvato)");
                    ImGui::EndTabItem();
                }

                if (tab3_open && ImGui::BeginTabItem("Tab 3", &tab3_open)) {
                    ImGui::Text("Contenuto tab 3");
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }
}