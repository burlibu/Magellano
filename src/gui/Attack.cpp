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
  AttackWindow::AttackWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window("Attack Window", p, s, win, f) {

    }
    void AttackWindow::Render() {
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::Begin("Attack", &bool_esperimenti_window, flags_esperimenti_window);

        static std::vector<std::string> errors;

        // Input text
        ImGui::SetNextItemWidth(150); // Imposta la larghezza desiderata in pixel
        static char ip[32] = "";
        ImGui::InputText("Target Ip", ip, IM_ARRAYSIZE(ip));
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150); // Imposta la larghezza desiderata in pixel
        static char port_str[8] = "";
        ImGui::InputText("Target port", port_str, IM_ARRAYSIZE(port_str));
        // Conversione sicura
        int port = 0;
        if (strlen(port_str) > 0 && std::all_of(port_str, port_str + strlen(port_str), ::isdigit)) {
            port = std::stoi(port_str);
            // Usa port come intero
        } else {
            errors.push_back("Invalid port number");
        }

        // Checkbox
        static bool claymore_op = false;
        ImGui::Checkbox("Deploy claymore (if target tries to modify or edit the payload the system will collapse)", &claymore_op);

        static bool spread_op = false;
        ImGui::Checkbox("Spread to nearby devices", &spread_op);

        static bool network_spread_op = false;
        ImGui::Checkbox("Try to attach the malware to the connected Network", &network_spread_op);

        // Radio buttons
        static const char* attack_types[] = { "Ddos", "Reverse Shell", "Malware Injection" };
        static int radio = 0;
        ImGui::RadioButton("Ddos Attack : deny connection and services to and from Rtarget by flooding it with junk traffic.", &radio, 0);
        ImGui::RadioButton("Reverse Shell : obtain access to the target operating system in order to gather information or modify it.", &radio, 1);
        ImGui::RadioButton("Malware injection : insert in the target's device a malicious code capable of incapacitating defenses or even destroy it.", &radio, 2);
        std::string attack_type = attack_types[radio];

        // // Color picker
        // static float color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
        // ImGui::ColorEdit4("Colore", color);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 20));
        if (ImGui::Button("Attack")) {
            int err[8] = {0}; // Inizializza il vettore degli errori con tutti zeri
            try
            {
              attack(ip, port, attack_type, claymore_op, spread_op, network_spread_op, err);
            }
            catch(const std::string& e)
            {
              errors.push_back(e);
            }
        }
        ImGui::PopStyleVar();
        if (errors.size() > 0) {
            ImGui::Text("Error: %s", errors.back().c_str());
        } 
        else {
            ImGui::Text("No errors occurred.");
        }

        ImGui::End();
    }
}