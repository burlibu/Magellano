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
#include <cstring>
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

// EXTENDED NET COUNTERS
struct NetCounters {
    unsigned long long rx_bytes = 0;
    unsigned long long rx_packets = 0;
    unsigned long long tx_bytes = 0;
    unsigned long long tx_packets = 0;
    // Additional error statistics
    unsigned long long rx_errors = 0;
    unsigned long long rx_dropped = 0;
    unsigned long long rx_fifo = 0;
    unsigned long long rx_frame = 0;
    unsigned long long tx_errors = 0;
    unsigned long long tx_dropped = 0;
    unsigned long long tx_fifo = 0;
    unsigned long long tx_colls = 0;
    unsigned long long tx_carrier = 0;
};

// TCP/UDP Connection structure
struct NetConnection {
    std::string protocol;  // "TCP" or "UDP"
    std::string state;     // ESTABLISHED, LISTEN, TIME_WAIT, etc.
    std::string local_addr;  // [127.0.0.1]:8080
    std::string remote_addr;  // [192.168.1.100]:12345
    unsigned int inode = 0;
};

// Protocol counters
struct ProtocolCounters {
    unsigned long long tcp_count = 0;
    unsigned long long udp_count = 0;
    unsigned long long established = 0;
    unsigned long long time_wait = 0;
    unsigned long long listen = 0;
};

// Convert hex string to IP address
std::string hex_to_ip(const std::string& hex) {
    if (hex.length() < 8) return hex;
    unsigned int ip = 0;
    std::sscanf(hex.c_str(), "%X", &ip);
    unsigned char* bytes = (unsigned char*)&ip;
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%u.%u.%u.%u", bytes[0], bytes[1], bytes[2], bytes[3]);
    return std::string(buf);
}

// Read TCP/UDP connections from /proc/net/tcp and /proc/net/udp
bool read_network_connections(std::vector<NetConnection>& out) {
    out.clear();
    
    // Read TCP connections
    std::ifstream tcp_file("/proc/net/tcp");
    if (tcp_file.is_open()) {
        std::string line;
        bool first = true;
        while (std::getline(tcp_file, line)) {
            if (first) {
                first = false;
                continue;  // Skip header
            }
            
            std::istringstream iss(line);
            int sl, tx_queue, rx_queue, tr_tm_when, retrnsmt, uid, timeout;
            std::string local_addr, rem_addr, st;
            unsigned int inode;
            
            if (iss >> sl >> local_addr >> rem_addr >> st >> tx_queue >> rx_queue >> tr_tm_when >> retrnsmt >> uid >> timeout >> inode) {
                NetConnection conn;
                conn.protocol = "TCP";
                
                if (st == "01") conn.state = "ESTABLISHED";
                else if (st == "0A") conn.state = "LISTEN";
                else if (st == "06") conn.state = "TIME_WAIT";
                else if (st == "04") conn.state = "TIME_WAIT";
                else conn.state = std::string("STATE_") + st;
                
                conn.local_addr = local_addr;
                conn.remote_addr = rem_addr;
                conn.inode = inode;
                out.push_back(conn);
            }
        }
        tcp_file.close();
    }
    
    // Read UDP connections
    std::ifstream udp_file("/proc/net/udp");
    if (udp_file.is_open()) {
        std::string line;
        bool first = true;
        while (std::getline(udp_file, line)) {
            if (first) {
                first = false;
                continue;  // Skip header
            }
            
            std::istringstream iss(line);
            int sl, tx_queue, rx_queue, tr_tm_when, retrnsmt, uid, timeout;
            std::string local_addr, rem_addr, st;
            unsigned int inode;
            
            if (iss >> sl >> local_addr >> rem_addr >> st >> tx_queue >> rx_queue >> tr_tm_when >> retrnsmt >> uid >> timeout >> inode) {
                NetConnection conn;
                conn.protocol = "UDP";
                conn.state = "N/A";
                conn.local_addr = local_addr;
                conn.remote_addr = rem_addr;
                conn.inode = inode;
                out.push_back(conn);
            }
        }
        udp_file.close();
    }
    
    return !out.empty();
}

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
        unsigned long long rx_compressed = 0, rx_multicast = 0;
        unsigned long long tx_carrier = 0, tx_compressed = 0;

        iss >> c.rx_bytes >> c.rx_packets >> c.rx_errors >> c.rx_dropped >> c.rx_fifo >> c.rx_frame
            >> rx_compressed >> rx_multicast >> c.tx_bytes >> c.tx_packets >> c.tx_errors
            >> c.tx_dropped >> c.tx_fifo >> c.tx_colls >> tx_carrier >> tx_compressed;

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

// ============= RENDER HELPER FUNCTIONS (inline in anonymous namespace) =============

inline void render_overview_tab(const std::map<std::string, NetCounters>& current,
                        const std::map<std::string, NetCounters>& previous,
                        double elapsed_s) {
    // Calculate totals
    unsigned long long total_rx = 0, total_tx = 0;
    unsigned long long total_rx_packets = 0, total_tx_packets = 0;
    unsigned long long total_errors = 0, total_dropped = 0;

    for (const auto& it : current) {
        total_rx += it.second.rx_bytes;
        total_tx += it.second.tx_bytes;
        total_rx_packets += it.second.rx_packets;
        total_tx_packets += it.second.tx_packets;
        total_errors += it.second.rx_errors + it.second.tx_errors;
        total_dropped += it.second.rx_dropped + it.second.tx_dropped;
    }

    // Statistics Box
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::CollapsingHeader("Global Statistics")) {
        ImGui::Columns(4, "overview_stats", true);
        
        ImGui::Text("Interfaces"); ImGui::NextColumn();
        ImGui::Text("Total RX"); ImGui::NextColumn();
        ImGui::Text("Total TX"); ImGui::NextColumn();
        ImGui::Text("Total Packets"); ImGui::NextColumn();
        ImGui::Separator();
        
        ImGui::Text("%d", (int)current.size()); ImGui::NextColumn();
        ImGui::Text("%.2f MB", static_cast<double>(total_rx) / (1024.0 * 1024.0)); ImGui::NextColumn();
        ImGui::Text("%.2f MB", static_cast<double>(total_tx) / (1024.0 * 1024.0)); ImGui::NextColumn();
        ImGui::Text("%llu", total_rx_packets + total_tx_packets); ImGui::NextColumn();
        
        ImGui::Columns(1);
    }

    ImGui::Spacing();

    // Error Statistics
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::CollapsingHeader("Error & Drop Statistics")) {
        ImGui::Columns(4, "error_stats", true);
        
        ImGui::Text("RX Errors"); ImGui::NextColumn();
        ImGui::Text("RX Dropped"); ImGui::NextColumn();
        ImGui::Text("TX Errors"); ImGui::NextColumn();
        ImGui::Text("TX Dropped"); ImGui::NextColumn();
        ImGui::Separator();
        
        unsigned long long rx_err = 0, tx_err = 0;
        for (const auto& it : current) {
            rx_err += it.second.rx_errors;
            tx_err += it.second.tx_errors;
        }
        
        ImGui::TextColored(total_errors > 0 ? ImVec4(1, 1, 0, 1) : ImVec4(0, 1, 0, 1), "%llu", rx_err);
        ImGui::NextColumn();
        ImGui::TextColored(total_dropped > 0 ? ImVec4(1, 0.5, 0, 1) : ImVec4(0, 1, 0, 1), "%llu", total_dropped);
        ImGui::NextColumn();
        ImGui::TextColored(tx_err > 0 ? ImVec4(1, 1, 0, 1) : ImVec4(0, 1, 0, 1), "%llu", tx_err);
        ImGui::NextColumn();
        ImGui::Text("---");
        ImGui::NextColumn();
        
        ImGui::Columns(1);
    }

    ImGui::Spacing();

    // Average packet size calculation
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::CollapsingHeader("Packet Statistics")) {
        double avg_rx_pkt = total_rx_packets > 0 ? (double)total_rx / total_rx_packets : 0;
        double avg_tx_pkt = total_tx_packets > 0 ? (double)total_tx / total_tx_packets : 0;
        double pps_total = elapsed_s > 0 ? (total_rx_packets + total_tx_packets) / elapsed_s : 0;
        
        ImGui::Text("Average RX Packet Size: %.2f bytes", avg_rx_pkt);
        ImGui::Text("Average TX Packet Size: %.2f bytes", avg_tx_pkt);
        ImGui::Text("Packets Per Second: %.0f pps", pps_total);
    }
}

inline void render_interface_details_tab(const std::map<std::string, NetCounters>& current,
                                                 const std::map<std::string, NetCounters>& previous,
                                                 double elapsed_s) {
    if (ImGui::BeginTable("interface_details", 9, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable)) {
        ImGui::TableSetupColumn("Interface", ImGuiTableColumnFlags_DefaultSort);
        ImGui::TableSetupColumn("RX (MB)");
        ImGui::TableSetupColumn("TX (MB)");
        ImGui::TableSetupColumn("RX Rate", ImGuiTableColumnFlags_NoHeaderLabel);
        ImGui::TableSetupColumn("TX Rate", ImGuiTableColumnFlags_NoHeaderLabel);
        ImGui::TableSetupColumn("RX Errors");
        ImGui::TableSetupColumn("RX Dropped");
        ImGui::TableSetupColumn("TX Errors");
        ImGui::TableSetupColumn("TX Collisions");
        ImGui::TableHeadersRow();

        for (const auto& it : current) {
            const std::string& iface = it.first;
            const NetCounters& c = it.second;

            double rx_rate = 0.0, tx_rate = 0.0;
            auto prev_it = previous.find(iface);
            if (prev_it != previous.end() && elapsed_s > 0.0) {
                rx_rate = static_cast<double>(c.rx_bytes - prev_it->second.rx_bytes) / elapsed_s;
                tx_rate = static_cast<double>(c.tx_bytes - prev_it->second.tx_bytes) / elapsed_s;
            }

            char rx_buf[32] = {0}, tx_buf[32] = {0};
            human_bytes_per_sec(rx_rate, rx_buf, sizeof(rx_buf));
            human_bytes_per_sec(tx_rate, tx_buf, sizeof(tx_buf));

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", iface.c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%.2f", static_cast<double>(c.rx_bytes) / (1024.0 * 1024.0));
            ImGui::TableSetColumnIndex(2); ImGui::Text("%.2f", static_cast<double>(c.tx_bytes) / (1024.0 * 1024.0));
            ImGui::TableSetColumnIndex(3); ImGui::Text("%s", rx_buf);
            ImGui::TableSetColumnIndex(4); ImGui::Text("%s", tx_buf);
            
            // RX Errors (color code)
            ImGui::TableSetColumnIndex(5);
            ImGui::TextColored(c.rx_errors > 0 ? ImVec4(1, 1, 0, 1) : ImVec4(0, 1, 0, 1), "%llu", c.rx_errors);
            
            // RX Dropped
            ImGui::TableSetColumnIndex(6);
            ImGui::TextColored(c.rx_dropped > 0 ? ImVec4(1, 0.5, 0, 1) : ImVec4(0, 1, 0, 1), "%llu", c.rx_dropped);
            
            // TX Errors
            ImGui::TableSetColumnIndex(7);
            ImGui::TextColored(c.tx_errors > 0 ? ImVec4(1, 1, 0, 1) : ImVec4(0, 1, 0, 1), "%llu", c.tx_errors);
            
            // TX Collisions
            ImGui::TableSetColumnIndex(8);
            ImGui::TextColored(c.tx_colls > 0 ? ImVec4(1, 0.2, 0, 1) : ImVec4(0, 1, 0, 1), "%llu", c.tx_colls);
        }

        ImGui::EndTable();
    }
}

inline void render_connections_tab(const std::vector<NetConnection>& connections) {
    ImGui::Text("Active TCP/UDP Connections: %d", (int)connections.size());
    ImGui::Separator();

    if (connections.empty()) {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "No active connections found.");
        return;
    }

    static ImGuiTextFilter filter;
    filter.Draw("Filter connections");

    if (ImGui::BeginTable("connections", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("Protocol");
        ImGui::TableSetupColumn("Local Address");
        ImGui::TableSetupColumn("Remote Address");
        ImGui::TableSetupColumn("State");
        ImGui::TableSetupColumn("Inode");
        ImGui::TableHeadersRow();

        for (const auto& conn : connections) {
            if (!filter.PassFilter(conn.local_addr.c_str()) && !filter.PassFilter(conn.remote_addr.c_str())) {
                continue;
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextColored(conn.protocol == "TCP" ? ImVec4(0, 1, 1, 1) : ImVec4(0, 1, 0.5, 1), "%s", conn.protocol.c_str());
            
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", conn.local_addr.c_str());
            ImGui::TableSetColumnIndex(2); ImGui::Text("%s", conn.remote_addr.c_str());
            ImGui::TableSetColumnIndex(3); ImGui::Text("%s", conn.state.c_str());
            ImGui::TableSetColumnIndex(4); ImGui::Text("%u", conn.inode);
        }

        ImGui::EndTable();
    }
}

inline void render_protocol_analysis_tab(const std::vector<NetConnection>& connections,
                                                 const std::map<std::string, NetCounters>& current) {
    // Protocol counters
    ProtocolCounters pc;
    std::map<std::string, int> port_stats;
    std::map<std::string, int> state_stats;

    for (const auto& conn : connections) {
        if (conn.protocol == "TCP") {
            pc.tcp_count++;
            if (conn.state == "ESTABLISHED") pc.established++;
            else if (conn.state == "LISTEN") pc.listen++;
            else if (conn.state == "TIME_WAIT") pc.time_wait++;
        } else {
            pc.udp_count++;
        }
        
        state_stats[conn.state]++;
        
        // Extract port from remote_addr (format: "192.168.1.1.12345" or similar)
        size_t last_dot = conn.remote_addr.rfind('.');
        if (last_dot != std::string::npos) {
            std::string port_str = conn.remote_addr.substr(last_dot + 1);
            port_stats[port_str]++;
        }
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::CollapsingHeader("Protocol Distribution")) {
        ImGui::Columns(2, "protocol_dist");
        ImGui::Text("TCP Connections"); ImGui::NextColumn();
        ImGui::Text("%llu", pc.tcp_count); ImGui::NextColumn();
        ImGui::Text("UDP Connections"); ImGui::NextColumn();
        ImGui::Text("%llu", pc.udp_count); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Text("ESTABLISHED"); ImGui::NextColumn();
        ImGui::Text("%llu", pc.established); ImGui::NextColumn();
        ImGui::Text("LISTEN"); ImGui::NextColumn();
        ImGui::Text("%llu", pc.listen); ImGui::NextColumn();
        ImGui::Text("TIME_WAIT"); ImGui::NextColumn();
        ImGui::Text("%llu", pc.time_wait); ImGui::NextColumn();
        ImGui::Columns(1);
    }

    ImGui::Spacing();

    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::CollapsingHeader("Connection States")) {
        if (ImGui::BeginTable("states", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("State");
            ImGui::TableSetupColumn("Count");
            ImGui::TableHeadersRow();

            for (const auto& state : state_stats) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", state.first.c_str());
                ImGui::TableSetColumnIndex(1); ImGui::Text("%d", state.second);
            }

            ImGui::EndTable();
        }
    }

    ImGui::Spacing();

    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::CollapsingHeader("Top Remote Ports")) {
        if (ImGui::BeginTable("ports", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Port");
            ImGui::TableSetupColumn("Connections");
            ImGui::TableHeadersRow();

            int count = 0;
            for (auto it = port_stats.rbegin(); it != port_stats.rend() && count < 20; ++it, ++count) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", it->first.c_str());
                ImGui::TableSetColumnIndex(1); ImGui::Text("%d", it->second);
            }

            ImGui::EndTable();
        }
    }
}

} // namespace (anonymous helper functions)

NetworkWindow::NetworkWindow(const std::string& title, ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, p, s, win, f) {}

void NetworkWindow::Render() {
    static std::map<std::string, NetCounters> previous;
    static std::map<std::string, NetCounters> current;
    static std::vector<NetConnection> connections;
    static std::vector<NetConnection> prev_connections;
    static auto last_sample = std::chrono::steady_clock::now();
    static auto last_conn_sample = std::chrono::steady_clock::now();
    static float refresh_seconds = 1.0f;
    static bool valid_data = false;

    const auto now = std::chrono::steady_clock::now();
    const double elapsed_s = std::chrono::duration<double>(now - last_sample).count();
    const double conn_elapsed_s = std::chrono::duration<double>(now - last_conn_sample).count();

    // Refresh network counters
    if (elapsed_s >= refresh_seconds || current.empty()) {
        previous = current;
        current.clear();
        valid_data = read_network_counters(current);
        last_sample = now;
    }

    // Refresh connections every 2 seconds
    if (conn_elapsed_s >= 2.0f || connections.empty()) {
        prev_connections = connections;
        connections.clear();
        read_network_connections(connections);
        last_conn_sample = now;
    }

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::Begin(title.c_str(), &bool_network, flags_network);

    ImGui::Text("Advanced Network Monitor - Wireshark-like Analysis");
    ImGui::SliderFloat("Refresh (s)", &refresh_seconds, 0.5f, 5.0f, "%.1f");
    ImGui::Separator();

    if (!valid_data) {
        ImGui::TextColored(rosso, "Unable to read network counters.");
        ImGui::End();
        return;
    }

    // TAB BAR
    if (ImGui::BeginTabBar("NetworkTabs")) {
        // TAB 1: OVERVIEW
        if (ImGui::BeginTabItem("Overview")) {
            render_overview_tab(current, previous, elapsed_s);
            ImGui::EndTabItem();
        }

        // TAB 2: INTERFACE DETAILS
        if (ImGui::BeginTabItem("Interface Details")) {
            render_interface_details_tab(current, previous, elapsed_s);
            ImGui::EndTabItem();
        }

        // TAB 3: ACTIVE CONNECTIONS
        if (ImGui::BeginTabItem("Active Connections")) {
            render_connections_tab(connections);
            ImGui::EndTabItem();
        }

        // TAB 4: PROTOCOL ANALYSIS
        if (ImGui::BeginTabItem("Protocol Analysis")) {
            render_protocol_analysis_tab(connections, current);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

} // namespace Gui
