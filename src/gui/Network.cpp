//standards
#include <algorithm>
#include <chrono>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
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
namespace {
struct NetCounters {
    unsigned long long rx_bytes = 0;
    unsigned long long rx_packets = 0;
    unsigned long long tx_bytes = 0;
    unsigned long long tx_packets = 0;
};

bool read_network_counters(std::map<std::string, NetCounters>& out) {
    std::ifstream in("/proc/net/dev");
    if (!in.is_open()) {
        return false;
    }

    std::string line;
    int line_no = 0;
    while (std::getline(in, line)) {
        ++line_no;
        if (line_no <= 2) {
            continue;
        }

        const std::size_t colon = line.find(':');
        if (colon == std::string::npos) {
            continue;
        }

        std::string iface = line.substr(0, colon);
        iface.erase(std::remove_if(iface.begin(), iface.end(), [](unsigned char c) { return std::isspace(c); }), iface.end());

        std::istringstream iss(line.substr(colon + 1));
        NetCounters c;
        unsigned long long rx_err = 0, rx_drop = 0, rx_fifo = 0, rx_frame = 0;
        unsigned long long rx_compressed = 0, rx_multicast = 0;
        unsigned long long tx_err = 0, tx_drop = 0, tx_fifo = 0, tx_colls = 0;
        unsigned long long tx_carrier = 0, tx_compressed = 0;

        iss >> c.rx_bytes >> c.rx_packets >> rx_err >> rx_drop >> rx_fifo >> rx_frame
            >> rx_compressed >> rx_multicast >> c.tx_bytes >> c.tx_packets >> tx_err
            >> tx_drop >> tx_fifo >> tx_colls >> tx_carrier >> tx_compressed;

        if (!iss.fail()) {
            out[iface] = c;
        }
    }

    return !out.empty();
}

const char* human_bytes_per_sec(double bytes_per_sec, char* buffer, std::size_t size) {
    const double kb = 1024.0;
    const double mb = kb * 1024.0;
    const double gb = mb * 1024.0;

    if (bytes_per_sec >= gb) {
        std::snprintf(buffer, size, "%.2f GB/s", bytes_per_sec / gb);
    } else if (bytes_per_sec >= mb) {
        std::snprintf(buffer, size, "%.2f MB/s", bytes_per_sec / mb);
    } else if (bytes_per_sec >= kb) {
        std::snprintf(buffer, size, "%.2f KB/s", bytes_per_sec / kb);
    } else {
        std::snprintf(buffer, size, "%.0f B/s", bytes_per_sec);
    }

    return buffer;
}
} // namespace

NetworkWindow::NetworkWindow(const std::string& title, ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, p, s, win, f) {}

void NetworkWindow::Render() {
    static std::map<std::string, NetCounters> previous;
    static std::map<std::string, NetCounters> current;
    static auto last_sample = std::chrono::steady_clock::now();
    static float refresh_seconds = 1.0f;
    static bool valid_data = false;

    const auto now = std::chrono::steady_clock::now();
    const double elapsed_s = std::chrono::duration<double>(now - last_sample).count();

    if (elapsed_s >= refresh_seconds || current.empty()) {
        previous = current;
        current.clear();
        valid_data = read_network_counters(current);
        last_sample = now;
    }

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::Begin(title.c_str(), &bool_network, flags_network);

    ImGui::Text("Real-time monitor from /proc/net/dev");
    ImGui::SliderFloat("Refresh (s)", &refresh_seconds, 0.5f, 5.0f, "%.1f");
    ImGui::Separator();

    if (!valid_data) {
        ImGui::TextColored(rosso, "Unable to read network counters.");
        ImGui::End();
        return;
    }

    unsigned long long total_rx = 0;
    unsigned long long total_tx = 0;
    for (std::map<std::string, NetCounters>::const_iterator it = current.begin(); it != current.end(); ++it) {
        total_rx += it->second.rx_bytes;
        total_tx += it->second.tx_bytes;
    }

    ImGui::Text("Interfaces: %d", static_cast<int>(current.size()));
    ImGui::Text("Total RX: %.2f MB", static_cast<double>(total_rx) / (1024.0 * 1024.0));
    ImGui::Text("Total TX: %.2f MB", static_cast<double>(total_tx) / (1024.0 * 1024.0));
    ImGui::Separator();

    if (ImGui::BeginTable("network_table", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("Interface");
        ImGui::TableSetupColumn("RX (MB)");
        ImGui::TableSetupColumn("TX (MB)");
        ImGui::TableSetupColumn("RX rate");
        ImGui::TableSetupColumn("TX rate");
        ImGui::TableSetupColumn("RX packets");
        ImGui::TableSetupColumn("TX packets");
        ImGui::TableHeadersRow();

        for (std::map<std::string, NetCounters>::const_iterator it = current.begin(); it != current.end(); ++it) {
            const std::string& iface = it->first;
            const NetCounters& c = it->second;

            double rx_rate = 0.0;
            double tx_rate = 0.0;

            std::map<std::string, NetCounters>::const_iterator prev = previous.find(iface);
            if (prev != previous.end() && elapsed_s > 0.0) {
                rx_rate = static_cast<double>(c.rx_bytes - prev->second.rx_bytes) / elapsed_s;
                tx_rate = static_cast<double>(c.tx_bytes - prev->second.tx_bytes) / elapsed_s;
            }

            char rx_buf[32] = {0};
            char tx_buf[32] = {0};

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", iface.c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%.2f", static_cast<double>(c.rx_bytes) / (1024.0 * 1024.0));
            ImGui::TableSetColumnIndex(2); ImGui::Text("%.2f", static_cast<double>(c.tx_bytes) / (1024.0 * 1024.0));
            ImGui::TableSetColumnIndex(3); ImGui::Text("%s", human_bytes_per_sec(rx_rate, rx_buf, sizeof(rx_buf)));
            ImGui::TableSetColumnIndex(4); ImGui::Text("%s", human_bytes_per_sec(tx_rate, tx_buf, sizeof(tx_buf)));
            ImGui::TableSetColumnIndex(5); ImGui::Text("%llu", c.rx_packets);
            ImGui::TableSetColumnIndex(6); ImGui::Text("%llu", c.tx_packets);
        }

        ImGui::EndTable();
    }

    ImGui::End();
}
} // namespace Gui
