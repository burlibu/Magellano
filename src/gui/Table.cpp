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
  Table::Table(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, pos, size, win, f) {}

    void Table::Render() {
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::Begin(title.c_str(), nullptr, flags_table);
        if (ImGui::BeginTable("TabellaAvanzata", 3, flags_table)) {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn("Nome");
            ImGui::TableSetupColumn("Valore");
            ImGui::TableHeadersRow();
        
            for (int row = 0; row < 5; ++row) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%d", row);
                ImGui::TableSetColumnIndex(1); ImGui::Text("Elemento %d", row);
                ImGui::TableSetColumnIndex(2); ImGui::Text("%.2f", row * 1.5f);
            }
        
            ImGui::EndTable();
        }
        ImGui::End();
    }
}