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
//json
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

namespace Gui {
  Table::Table(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, pos, size, win, f) {}

    void Table::Render() {
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        ImGui::Begin(title.c_str(), nullptr, flags_table);

        // Lettura degli attacchi dal file attacks.json
        std::vector<json> attacks;
        std::ifstream infile(attacks_file_path);
        if (infile.is_open()) {
            json attacks_json;
            try {
                infile >> attacks_json;
            } catch (const std::exception&) {
                attacks_json = json::array();
            }
            infile.close();
            if (attacks_json.is_array()) {
                for (auto& attack : attacks_json) {
                    attacks.push_back(attack);
                }
            }
        }

        const ImGuiTableFlags attack_log_table_flags =
            ImGuiTableFlags_Borders
            | ImGuiTableFlags_RowBg
            | ImGuiTableFlags_Resizable
            | ImGuiTableFlags_ScrollX
            | ImGuiTableFlags_ScrollY
            | ImGuiTableFlags_SizingFixedFit;

        if (ImGui::BeginTable("Attacks history", 9, attack_log_table_flags)) {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableSetupColumn("IP", ImGuiTableColumnFlags_WidthFixed, 130.0f);
            ImGui::TableSetupColumn("Port", ImGuiTableColumnFlags_WidthFixed, 65.0f);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableSetupColumn("Claymore", ImGuiTableColumnFlags_WidthFixed, 90.0f);
            ImGui::TableSetupColumn("Spread", ImGuiTableColumnFlags_WidthFixed, 80.0f);
            ImGui::TableSetupColumn("Network Spread", ImGuiTableColumnFlags_WidthFixed, 125.0f);
            ImGui::TableSetupColumn("Timestamp", ImGuiTableColumnFlags_WidthFixed, 170.0f);
            ImGui::TableSetupColumn("Result", ImGuiTableColumnFlags_WidthFixed, 80.0f);
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();

            for (size_t row = 0; row < attacks.size(); ++row) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); //Seleziona la prima colonna (indice 0) della riga corrente nella tabella.
                ImGui::Text("%zu", row+1); //Scrive il numero della riga (partendo da 1) nella cella selezionata.
                ImGui::TableSetColumnIndex(1); ImGui::Text("%s", attacks[row]["ip"].get<std::string>().c_str());
                ImGui::TableSetColumnIndex(2); ImGui::Text("%d", attacks[row]["port"].get<int>());
                ImGui::TableSetColumnIndex(3); ImGui::Text("%s", attacks[row]["type"].get<std::string>().c_str());
                        // Gestione sicura dei campi booleani
                        bool claymore = attacks[row].contains("claymore") && !attacks[row]["claymore"].is_null() ? attacks[row]["claymore"].get<bool>() : false;
                        bool spread = attacks[row].contains("spread") && !attacks[row]["spread"].is_null() ? attacks[row]["spread"].get<bool>() : false;
                        bool network_spread = attacks[row].contains("network_spread") && !attacks[row]["network_spread"].is_null() ? attacks[row]["network_spread"].get<bool>() : false;
                        bool success = attacks[row].contains("success") && !attacks[row]["success"].is_null() ? attacks[row]["success"].get<bool>() : false;
                        std::string timestamp = attacks[row].contains("timestamp") && !attacks[row]["timestamp"].is_null() ? attacks[row]["timestamp"].get<std::string>() : "0";
                        ImGui::TableSetColumnIndex(4); ImGui::Text("%s", claymore ? "True" : "False");
                        ImGui::TableSetColumnIndex(5); ImGui::Text("%s", spread ? "True" : "False");
                        ImGui::TableSetColumnIndex(6); ImGui::Text("%s", network_spread ? "True" : "False");
                        ImGui::TableSetColumnIndex(7); ImGui::Text("%s", timestamp.c_str());
                        ImGui::TableSetColumnIndex(8);
                        if (success) {
                            ImGui::PushStyleColor(ImGuiCol_Text, verde); //+ verde
                            ImGui::Text("Success");
                            ImGui::PopStyleColor();
                        } else {
                            ImGui::PushStyleColor(ImGuiCol_Text, rosso); //! rosso
                            ImGui::Text("Fail");
                            ImGui::PopStyleColor();
                        }
            }

            ImGui::EndTable();
        }
        ImGui::End();
    }
}