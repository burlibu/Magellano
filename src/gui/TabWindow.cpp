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
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        if (ImGui::Begin(title.c_str(), nullptr, flags_tab_window)) {
            if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_Reorderable)) {
                // Tab fisso (non chiudibile)
                if (ImGui::BeginTabItem("System")) {
                    ImGui::Text("System info");
                    
                    SystemInfo sysInfo = get_system_info();
                    
                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.4f, 1.0f), "Device: %s", sysInfo.deviceName.c_str());
                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.4f, 1.0f), "OS: %s %s", sysInfo.osName.c_str(), sysInfo.osVersion.c_str());
                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.4f, 1.0f), "CPU: %s", sysInfo.cpuInfo.c_str());
                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 0.4f, 1.0f), "RAM: %zu MB (Free: %zu MB)", 
                                      sysInfo.totalRamMB, sysInfo.freeRamMB);
                    ImGui::Separator();
                    
                    float ramUsagePercentage = 100.0f * (1.0f - static_cast<float>(sysInfo.freeRamMB) / static_cast<float>(sysInfo.totalRamMB));
                    ImGui::Text("RAM Usage: %.1f%%", ramUsagePercentage);
                    ImGui::ProgressBar(ramUsagePercentage / 100.0f, ImVec2(-1, 0), 
                                      ("Used: " + std::to_string(sysInfo.totalRamMB - sysInfo.freeRamMB) + " MB").c_str());
                    
                    if (ImGui::Button("Refresh System Info")) {
                        // La struttura sysInfo verrà aggiornata automaticamente al prossimo ciclo di rendering
                    }
                    ImGui::EndTabItem();
                }

                // Tab chiudibili
                if (tab1_open && ImGui::BeginTabItem("Logs", &tab1_open)) {

                    ImGui::Text("Logs");
                    ImGui::EndTabItem();
                }

                if (tab2_open && ImGui::BeginTabItem("Proxy", &tab2_open, ImGuiTabItemFlags_UnsavedDocument)) {
                    ImGui::Text("Contenuto tab 2 (non salvato)");
                    ImGui::EndTabItem();
                }

                if (tab3_open && ImGui::BeginTabItem("Network", &tab3_open)) {
                    ImGui::Text("Network monitoring and diagnostycs");
                    ImGui::EndTabItem();
                }

                if (tab4_open && ImGui::BeginTabItem("Firewall", &tab3_open)) {
                    ImGui::Text("Firewall options");
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }
}