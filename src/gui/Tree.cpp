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
  Tree::Tree(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, pos, size, win, f) {}

    void Tree::Render() {
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        ImGui::Begin(title.c_str(), nullptr, flags_tree);
        if (ImGui::TreeNode("Nodo Principale")) {
            ImGui::Text("Contenuto del nodo");
            if (ImGui::TreeNode("Elenco Progetti")) {
                ImGui::Text("[Cariddi #1] Alberus");
                ImGui::Text("[Cariddi #2] Valkyrie");
                ImGui::Text("[Cariddi #3] Jerico");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Sotto-nodo 2")) {
                ImGui::Text("Contenuto del sotto-nodo 2");
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        ImGui::End();

    }
}