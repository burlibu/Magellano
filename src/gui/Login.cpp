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
  LoginWindow::LoginWindow(ImVec2 position, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f) // inizializzazione costruttore Window
        : Window("Login", position, size, win, f) {} // gli sta passando questi parametri

    /**
     *  Parameters:
     * 
     * `const ImVec2& cursorpos` posizione del cursore dove sarà l' angolo alto sinistro della schermata
     * `const ImVec2& window_size` larghezza e altezza in un imvec2
     * Attenzione: chiama checkWindowSize se DEBUG è attivo
     */
    void LoginWindow::Render() {
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        if (ImGui::Begin(title.c_str(), &isOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings)) {
            ImGui::InputText("Username", username, IM_ARRAYSIZE(username));
            ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
            if (ImGui::Button("Login")) {
                if (std::string(username) == "admin" && std::string(password) == "password") {
                    login_success = true;
                } else {
                    login_success = false;
                }
            }
            if (login_success) ImGui::TextColored(ImVec4(0,1,0,1), "Login Successful");
            else ImGui::TextColored(ImVec4(1,0,0,1), "Login Failed");
        }
        ImGui::End();
    } // fine funzione Render

    void LoginWindow::isLogged(){
        if (login_success) {
            logged = true;
        } else logged = false;
    }
}