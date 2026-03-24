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
  MinimizeAndExitWindow::MinimizeAndExitWindow(ImVec2 position, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f) 
        : Window("MinimizeAndExitWindow", position, size, win, f) {};

    void MinimizeAndExitWindow::Render() {
        // Aggiorna la larghezza della barra in base alla finestra principale
        int width, height;
        glfwGetFramebufferSize(window_ptr, &width, &height);
        size.x = width; // aggiorna la larghezza della barra
        // Mantieni la posizione sempre in alto a sinistra
        pos = ImVec2(0, 0);

        ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        ImGui::Begin("minimize_and_exit_window_id", &isOpen, flags_minimize_and_exit_window);
        //
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4)); // padding interno dei bottoni (il blu)
        // Posiziona i bottoni a destra
        float windowWidth = ImGui::GetWindowWidth();
        ImGui::SetCursorPosX(windowWidth - ImGui::CalcTextSize("- X").x - 30);
        // Bottone Minimize
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, arancione); // hover arancione
        ImGui::PushStyleColor(ImGuiCol_Text, bianco); // -testo bianco
        if (ImGui::Button("-")) {
            glfwHideWindow(window_ptr); 
        }
        ImGui::PopStyleColor(2);
        ImGui::SameLine();

        // Bottone Exit
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, rosso); // hover rosso
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1)); // testo bianco
        if (ImGui::Button("X")) {
            glfwSetWindowShouldClose(window_ptr, GLFW_TRUE);
        }
        checkWindowSizeChange(size);
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
        ImGui::End();
    } // fine MinimizeAndExitWindow::Render()
}