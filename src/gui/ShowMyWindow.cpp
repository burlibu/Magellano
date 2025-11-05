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
  ShowMyWindow::ShowMyWindow(ImVec2 position, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f) 
        : Window("My window", position, size, win, f) {}
    /**
     *  Parameters:
     * 
     * `const ImVec2& cursorpos` posizione del cursore dove sarà l' angolo alto sinistro della schermata
     * `const ImVec2& window_size` larghezza e altezza in un imvec2
     * Attenzione: chiama checkWindowSize se DEBUG è attivo
     */
    void ShowMyWindow::Render() {
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::Begin("MyWindow", &isOpen);
        ImGui::Text("Benvenuto nel mio progetto ImGui!");
        ImGui::Separator();
        
        ImGui::SliderFloat("Float Value", &my_float, 0.0f, 1.0f);
        ImGui::SliderInt("Int Value", &my_int, 0, 100);
        
        if (ImGui::Button("Click Me!")) {
            std::cout << "Bottone cliccato!" << std::endl;
            count++;
        }
        
        ImGui::SameLine();
        ImGui::Text("Counter: %d", count);
        
        ImGui::End();

        if (DEBUG) {
        
        }
    }
}