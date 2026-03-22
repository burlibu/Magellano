//standards
#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <filesystem>
#include <sys/statvfs.h>
#include <system_error>
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

namespace fs = std::filesystem;

namespace Gui {

// HELPER FUNCTIONS
namespace {

bool read_system_stats(SystemStats& stats) {
    // Read /proc/uptime
    std::ifstream uptime_file("/proc/uptime");
    if (uptime_file.is_open()) {
        double uptime;
        uptime_file >> uptime;
        stats.uptime_seconds = (unsigned long)uptime;
        uptime_file.close();
    }

    // Read /proc/meminfo
    std::ifstream meminfo_file("/proc/meminfo");
    if (meminfo_file.is_open()) {
        std::string line;
        while (std::getline(meminfo_file, line)) {
            std::istringstream iss(line);
            std::string key;
            unsigned long value;
            if (iss >> key >> value) {
                if (key == "MemTotal:") stats.total_ram_mb = value / 1024;
                if (key == "MemAvailable:") stats.free_ram_mb = value / 1024;
            }
        }
        meminfo_file.close();
        stats.used_ram_mb = stats.total_ram_mb - stats.free_ram_mb;
        stats.ram_usage = stats.total_ram_mb > 0 ? (100.0f * stats.used_ram_mb) / stats.total_ram_mb : 0.0f;
    }

    // Read /proc/stat for CPU usage
    static unsigned long prev_total = 0, prev_idle = 0;
    std::ifstream stat_file("/proc/stat");
    if (stat_file.is_open()) {
        std::string line;
        std::getline(stat_file, line);
        std::istringstream iss(line);
        std::string cpu_str;
        unsigned long user, nice, system, idle;
        iss >> cpu_str >> user >> nice >> system >> idle;
        
        unsigned long total = user + nice + system + idle;
        unsigned long total_diff = total - prev_total;
        unsigned long idle_diff = idle - prev_idle;
        
        if (total_diff > 0) {
            stats.cpu_usage = 100.0f * (1.0f - (float)idle_diff / (float)total_diff);
        }
        
        prev_total = total;
        prev_idle = idle;
        stat_file.close();
    }

    // Disk usage (root partition)
    struct statvfs vfs;
    if (statvfs("/", &vfs) == 0) {
        unsigned long total_space = vfs.f_blocks * vfs.f_frsize;
        unsigned long free_space = vfs.f_bavail * vfs.f_frsize;
        unsigned long used_space = total_space - free_space;
        stats.disk_usage = (100.0f * used_space) / total_space;
    }

    return true;
}

bool read_top_processes(std::vector<ProcessInfo>& processes, int max_count = 20) {
    processes.clear();
    
    try {
        for (const auto& entry : fs::directory_iterator("/proc")) {
            if (!fs::is_directory(entry)) continue;
            
            std::string dir_name = entry.path().filename().string();
            if (dir_name[0] < '0' || dir_name[0] > '9') continue;
            
            int pid = std::stoi(dir_name);
            std::string cmdline_path = entry.path().string() + "/cmdline";
            std::string stat_path = entry.path().string() + "/stat";
            
            // Read process name
            std::ifstream cmdline_file(cmdline_path);
            std::string name;
            if (cmdline_file.is_open()) {
                std::getline(cmdline_file, name, '\0');
                if (name.empty()) {
                    // Fallback to stat file
                    std::ifstream stat_file(stat_path);
                    if (stat_file.is_open()) {
                        std::string line;
                        std::getline(stat_file, line);
                        size_t start = line.find('(') + 1;
                        size_t end = line.find(')');
                        if (start != std::string::npos && end != std::string::npos) {
                            name = line.substr(start, end - start);
                        }
                        stat_file.close();
                    }
                }
                cmdline_file.close();
            }
            
            if (!name.empty()) {
                // Read memory usage
                std::ifstream status_file(entry.path().string() + "/status");
                unsigned long rss = 0;
                if (status_file.is_open()) {
                    std::string line;
                    while (std::getline(status_file, line)) {
                        if (line.find("VmRSS:") != std::string::npos) {
                            std::istringstream iss(line);
                            std::string key;
                            unsigned long value;
                            iss >> key >> value;
                            rss = value / 1024;  // Convert to MB
                            break;
                        }
                    }
                    status_file.close();
                }
                
                ProcessInfo proc;
                proc.pid = pid;
                proc.name = name;
                proc.rss_mb = rss;
                proc.state = "running";
                processes.push_back(proc);
            }
        }
    } catch (...) {
        return false;
    }
    
    // Sort by memory usage descending
    std::sort(processes.begin(), processes.end(),
        [](const ProcessInfo& a, const ProcessInfo& b) {
            return a.rss_mb > b.rss_mb;
        });
    
    if (processes.size() > (size_t)max_count) {
        processes.resize(max_count);
    }
    
    return !processes.empty();
}

} // anonymous namespace

  TabWindow::TabWindow(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, pos, size, win, f) {}

void TabWindow::Render(){
    static SystemStats stats;
    static std::vector<ProcessInfo> processes;
    static std::vector<std::string> log_entries;
    static auto last_stats_update = std::chrono::steady_clock::now();
    static auto last_proc_update = std::chrono::steady_clock::now();
    static std::string current_directory = "/home";
    static int selected_process = -1;
    
    const auto now = std::chrono::steady_clock::now();
    
    // Update stats every 1 second
    if (std::chrono::duration<double>(now - last_stats_update).count() > 1.0) {
        read_system_stats(stats);
        last_stats_update = now;
    }
    
    // Update processes every 2 seconds
    if (std::chrono::duration<double>(now - last_proc_update).count() > 2.0) {
        read_top_processes(processes, 30);
        last_proc_update = now;
    }
    
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    if (ImGui::Begin(title.c_str(), nullptr, flags_tab_window)) {
        if (ImGui::BeginTabBar("SystemTabBar", ImGuiTabBarFlags_Reorderable)) {
            
            // TAB 1: SYSTEM STATS
            if (ImGui::BeginTabItem("System Stats")) {
                render_system_stats_tab(stats);
                ImGui::EndTabItem();
            }
            
            // TAB 2: PROCESS MONITOR
            if (ImGui::BeginTabItem("Processes")) {
                render_process_monitor_tab(processes, selected_process);
                ImGui::EndTabItem();
            }
            
            // TAB 3: FILE BROWSER
            if (ImGui::BeginTabItem("File Browser")) {
                render_file_browser_tab(current_directory);
                ImGui::EndTabItem();
            }
            
            // TAB 4: APPLICATION INFO
            if (ImGui::BeginTabItem("Info")) {
                render_info_tab();
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void TabWindow::render_system_stats_tab(const SystemStats& stats) {
    // CPU Usage Bar
    ImGui::Text("CPU Usage");
    ImGui::ProgressBar(stats.cpu_usage / 100.0f, ImVec2(-1, 0));
    ImGui::SameLine(); ImGui::Text("%.1f%%", stats.cpu_usage);
    
    ImGui::Spacing();
    
    // Memory Usage
    ImGui::Text("Memory Usage");
    ImGui::ProgressBar(stats.ram_usage / 100.0f, ImVec2(-1, 0));
    ImGui::SameLine(); ImGui::Text("%lu / %lu MB (%.1f%%)", stats.used_ram_mb, stats.total_ram_mb, stats.ram_usage);
    
    ImGui::Spacing();
    
    // Disk Usage
    ImGui::Text("Disk Usage (/)");
    ImVec4 disk_color = stats.disk_usage > 90.0f ? ImVec4(1, 0, 0, 1) : 
                        stats.disk_usage > 70.0f ? ImVec4(1, 1, 0, 1) : ImVec4(0, 1, 0, 1);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, disk_color);
    ImGui::ProgressBar(stats.disk_usage / 100.0f, ImVec2(-1, 0));
    ImGui::PopStyleColor();
    ImGui::SameLine(); ImGui::TextColored(disk_color, "%.1f%%", stats.disk_usage);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Uptime
    unsigned long hours = stats.uptime_seconds / 3600;
    unsigned long minutes = (stats.uptime_seconds % 3600) / 60;
    unsigned long seconds = stats.uptime_seconds % 60;
    
    ImGui::Text("System Uptime: %lu h %lu m %lu s", hours, minutes, seconds);
    ImGui::Text("Free RAM: %lu MB", stats.free_ram_mb);
}

void TabWindow::render_process_monitor_tab(const std::vector<ProcessInfo>& processes, int& selected) {
    ImGui::Text("Top Processes by Memory Usage");
    ImGui::Separator();
    
    if (ImGui::BeginTable("processes_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("PID");
        ImGui::TableSetupColumn("Process Name");
        ImGui::TableSetupColumn("Memory (MB)");
        ImGui::TableSetupColumn("Status");
        ImGui::TableHeadersRow();
        
        for (size_t i = 0; i < processes.size(); ++i) {
            const auto& proc = processes[i];
            
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%d", proc.pid);
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", proc.name.c_str());
            ImGui::TableSetColumnIndex(2); ImGui::TextColored(
                proc.rss_mb > 500 ? ImVec4(1, 0.5, 0, 1) : ImVec4(0, 1, 0.5, 1),
                "%lu", proc.rss_mb);
            ImGui::TableSetColumnIndex(3); ImGui::Text("%s", proc.state.c_str());
        }
        
        ImGui::EndTable();
    }
}

void TabWindow::render_file_browser_tab(std::string& current_dir) {
    ImGui::Text("File Browser");
    static char path_buffer[512] = {0};
    std::strncpy(path_buffer, current_dir.c_str(), sizeof(path_buffer) - 1);
    path_buffer[sizeof(path_buffer) - 1] = '\0';
    ImGui::InputText("Current Path##filebrowser", path_buffer, sizeof(path_buffer), ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();
    if (ImGui::Button("Refresh")) {
        // Refresh file list
    }
    
    ImGui::Separator();
    
    if (ImGui::Button("../")) {
        fs::path p(current_dir);
        if (p.has_parent_path()) {
            current_dir = p.parent_path().string();
        }
    }

    std::error_code ec;
    if (!fs::exists(current_dir, ec) || !fs::is_directory(current_dir, ec)) {
        ImGui::TextColored(rosso, "Invalid directory: %s", current_dir.c_str());
        return;
    }

    if (ImGui::BeginTable("file_browser", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Size");
        ImGui::TableHeadersRow();

        for (const auto& entry : fs::directory_iterator(current_dir, fs::directory_options::skip_permission_denied, ec)) {
            if (ec) {
                ec.clear();
                continue;
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            const bool is_dir = entry.is_directory(ec);
            if (ec) {
                ec.clear();
            }

            const std::string name = entry.path().filename().string();
            if (is_dir) {
                ImGui::TextColored(ImVec4(0, 1, 1, 1), "[DIR] %s", name.c_str());
            } else {
                ImGui::Text("%s", name.c_str());
            }

            if (ImGui::IsItemClicked() && is_dir) {
                current_dir = entry.path().string();
            }

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", is_dir ? "Directory" : "File");

            ImGui::TableSetColumnIndex(2);
            if (!is_dir) {
                const auto size = entry.file_size(ec);
                if (!ec) {
                    ImGui::Text("%llu KB", static_cast<unsigned long long>(size / 1024));
                } else {
                    ImGui::Text("N/A");
                    ec.clear();
                }
        }
        }

        ImGui::EndTable();
    }
}

void TabWindow::render_info_tab() {
    ImGui::Text("Application Information");
    ImGui::Separator();
    
    ImGui::Text("Application: Magellano");
    ImGui::Text("Version: 0.2.0-crossplatform");
    ImGui::Text("Build: CMake-based cross-platform");
    ImGui::Text("Platform: Linux");
    ImGui::Text("ImGui Version: %s", IMGUI_VERSION);
    
    ImGui::Spacing();
    ImGui::Text("Resources");
    ImGui::BulletText("Configuration: ~/.magellano/");
    ImGui::BulletText("Data: ./data/");
    ImGui::BulletText("Logs: ./logs/");
}

    } // namespace Gui