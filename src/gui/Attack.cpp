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
  AttackWindow::AttackWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window("Attack Window", p, s, win, f) {

    }
    void AttackWindow::StartScan(const std::string& ip, int port) {
    scan_in_progress = true;
    scan_progress = 0.0f;
    scan_results.clear();

    std::thread([ip, port]() {
        try {
            while (scan_progress < 1.0f) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simula il tempo di scansione
                scan_progress += 0.01f;
            }
            // Simula risultati dello scan
            scan_results.push_back("Port " + std::to_string(port) + " is open.");
            scan_results.push_back("No vulnerabilities found.");
        } catch (const std::exception& e) {
            scan_results.push_back(std::string("Error: ") + e.what());
        }
        scan_in_progress = false;
    }).detach();
}

void AttackWindow::RenderScanUI() {
    if (scan_in_progress) {
        ImGui::Text("Scanning...");
        ImGui::ProgressBar(scan_progress, ImVec2(0.0f, 0.0f));
    } else if (!scan_results.empty()) {
        ImGui::Text("Scan completed!");
        for (const auto& result : scan_results) {
            ImGui::Text("%s", result.c_str());
        }
    } else {
        ImGui::Text("Ready to scan.");
    }
}

void AttackWindow::Render() {
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::Begin("Attack", &bool_attack_window, flags_attack_window);

        static std::vector<std::string> errors;
        ImGui::Text("Scan Target:");
        // Input text
        ImGui::SetNextItemWidth(150); // Imposta la larghezza desiderata in pixel
        static char ip[32] = "";
        ImGui::InputText("Target Ip", ip, IM_ARRAYSIZE(ip));
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150); // Imposta la larghezza desiderata in pixel
        static char port_str[8] = "";
        ImGui::InputText("Target port", port_str, IM_ARRAYSIZE(port_str)); db(port_str);
        // Conversione sicura
        int port = 0;
        if (strlen(port_str) > 0 && std::all_of(port_str, port_str + strlen(port_str), ::isdigit)) {
            port = std::stoi(port_str);
            db("[Port OK: port_int: ", port, "port_str: ", port_str, " ]");
            // Usa port come intero
        } else {
            errors.push_back("Invalid port number");
            db("[Port ERROR: ", port, "port_str: ", port_str, " ]");

        }
        float scan_progress = 0.0f;
        /// TODO creare barra di progresso per lo scanning
        if(ImGui::Button("Start Scan")) {
            // Avvia lo scan
            errors.clear(); // Pulisce gli errori precedenti
            // scan_target(std::string(ip), port, scan_results, err);
            while (scan_progress < 1.0f) {
                scan_progress += 0.01f; // Simula il progresso
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simula il tempo di scansione
                ImGui::ProgressBar(scan_progress, ImVec2(0.0f, 0.0f), "Scanning...");
            }
        }
        ImGui::Separator();
        ImGui::Text("Scan completed!");
        // TODO mostra risultati scansione con informazioni dispositivo e vulnerabilità trovate
        ImGui::Separator();
        ImGui::Text("Launch Attack:");


        // Checkbox
        static bool claymore_op = false;
        ImGui::Checkbox("Deploy claymore (if target tries to modify or edit the payload the system will collapse)", &claymore_op);

        static bool spread_op = false;
        ImGui::Checkbox("Spread to nearby devices", &spread_op);

        static bool network_spread_op = false;
        ImGui::Checkbox("Infect Network", &network_spread_op);

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
            if (DEBUG) cout << errors.back().c_str();
        } 
        else {
            ImGui::Text("No errors occurred.");
        }

        ImGui::End();
    }
}