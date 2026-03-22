//standards
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>
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
struct ChangelogState {
    bool fetched = false;
    bool ok = false;
    std::string content;
    std::string error;
    std::string source_url;
};

std::string trim_copy(const std::string& text) {
    std::string out = text;
    while (!out.empty() && (out.back() == '\n' || out.back() == '\r')) {
        out.pop_back();
    }
    return out;
}

bool command_exists(const std::string& cmd) {
#ifdef _WIN32
    // Windows: use 'where' command to find executables in PATH
    const std::string check = "where " + cmd + " >nul 2>&1";
    return std::system(check.c_str()) == 0;
#else
    // Linux/Unix: use 'command -v' to check if command exists
    const std::string check = "command -v " + cmd + " >/dev/null 2>&1";
    return std::system(check.c_str()) == 0;
#endif
}

bool run_command_capture(const std::string& command, std::string& output, int& exit_code) {
    output.clear();
    exit_code = -1;

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        output = "Unable to spawn process.";
        return false;
    }

    std::array<char, 512> buffer;
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
        output += buffer.data();
    }

    const int status = pclose(pipe);
    if (status == -1) {
        output += "\nUnable to read process status.";
        return false;
    }

    exit_code = status;
    return true;
}

bool fetch_changelog_from_url(const std::string& url, std::string& content, std::string& error) {
    std::string output;
    int exit_code = -1;
    const std::string command = "curl -fsSL --connect-timeout 5 --max-time 15 \"" + url + "\" 2>&1";

    if (!run_command_capture(command, output, exit_code)) {
        error = "Process error while executing curl.";
        if (!output.empty()) {
            error += " Details: " + trim_copy(output);
        }
        return false;
    }

    if (exit_code != 0) {
        error = "Network error while downloading changelog.";
        if (!output.empty()) {
            error += " Details: " + trim_copy(output);
        }
        return false;
    }

    if (output.empty()) {
        error = "Download completed but changelog is empty.";
        return false;
    }

    content = output;
    return true;
}

void fetch_changelog(ChangelogState& state) {
    state.fetched = true;
    state.ok = false;
    state.content.clear();
    state.error.clear();
    state.source_url.clear();

    if (!command_exists("curl")) {
#ifdef _WIN32
        state.error = "curl is not available on this system.\n"
                      "Please install curl:\n"
                      "1. Install MSVC or use vcpkg: vcpkg install curl:x64-windows\n"
                      "2. Or use Chocolatey: choco install curl\n"
                      "3. Or download from: https://curl.se/windows/";
#else
        state.error = "curl is not available on this system.\n"
                      "Please install curl using your package manager:\n"
                      "Ubuntu/Debian: sudo apt-get install curl\n"
                      "Fedora/RHEL: sudo dnf install curl\n"
                      "macOS: brew install curl";
#endif
        return;
    }

    const std::string url_main = "https://raw.githubusercontent.com/burlibu/Magellano/refs/heads/main/CHANGELOG.md";
    const std::string url_master = "https://raw.githubusercontent.com/burlibu/Magellano/refs/heads/master/CHANGELOG.md";

    std::string content;
    std::string error;

    if (fetch_changelog_from_url(url_main, content, error)) {
        state.ok = true;
        state.content = content;
        state.source_url = url_main;
        return;
    }

    const std::string first_error = error;
    if (fetch_changelog_from_url(url_master, content, error)) {
        state.ok = true;
        state.content = content;
        state.source_url = url_master;
        return;
    }

    state.error = "Unable to fetch changelog from remote repository.";
    state.error += "\nmain error: " + first_error;
    state.error += "\nmaster error: " + error;
}
} // namespace

ChangelogWindow::ChangelogWindow(const std::string& title, ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, p, s, win, f) {}

void ChangelogWindow::Render() {
    static ChangelogState state;

    if (!state.fetched) {
        fetch_changelog(state);
    }

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    if (ImGui::Begin(title.c_str(), &bool_changelog, flags_changelog)) {
        ImGui::TextUnformatted("Remote source: https://github.com/burlibu/Magellano.git");
        ImGui::SameLine();
        if (ImGui::Button("Refresh")) {
            fetch_changelog(state);
        }

        ImGui::Separator();

        if (state.ok) {
            if (!state.source_url.empty()) {
                ImGui::Text("Fetched from: %s", state.source_url.c_str());
                ImGui::Separator();
            }

            if (ImGui::BeginChild("ChangelogContent", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_HorizontalScrollbar)) {
                ImGui::TextUnformatted(state.content.c_str());
            }
            ImGui::EndChild();
        } else {
            ImGui::PushStyleColor(ImGuiCol_Text, rosso);
            ImGui::TextUnformatted("Failed to load CHANGELOG.md from remote repository.");
            ImGui::PopStyleColor();
            if (!state.error.empty()) {
                ImGui::Spacing();
                ImGui::TextWrapped("%s", state.error.c_str());
            }
        }
    }
    ImGui::End();
}
} // namespace Gui
