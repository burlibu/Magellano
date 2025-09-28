//standard
#include <iostream>
#include <sstream>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include <random>
#include <regex>
#include <ctime>
#include <algorithm>

//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
//src
#include "env.h"
#include "settings.h"
#include "functions.h"
#include "custom_colors.h"
//json
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;
//windows
#ifdef _WIN32
#include <windows.h>
#include <sysinfoapi.h>
#else//linux
#include <sys/utsname.h>
#include <unistd.h>
#endif
struct Notification {
    std::string title;
    std::string text;
    float duration;
    std::chrono::steady_clock::time_point start_time;
};

std::vector<Notification> notifications; // creazione vector di notifiche

void setBool(bool &settings_bool, bool valuer) {
  settings_bool = valuer; // Simply set the boolean to the new value
}

void showNotification(const std::string& title, const std::string& text, int duration_ms) {
    Notification notification;
    notification.title = title;
    notification.text = text;
    notification.duration = duration_ms / 1000.0f; // dividiamo per mille così è in secondi
    notification.start_time = std::chrono::steady_clock::now();
    notifications.push_back(notification); // aggiunta della notifica al vettore di notifiche
}

// funzione che crea una finestra di notifica sullo schermo
void RenderNotifications() {
    ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x-250, ImGui::GetIO().DisplaySize.y- 100), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(240,80), ImGuiCond_Always);

    for (long unsigned int i = 0; i < notifications.size();) {
        Notification& notification = notifications[i];
        float elapsed = std::chrono::duration<float>(std::chrono::steady_clock::now() - notification.start_time).count();

        if (elapsed > notification.duration) {
            notifications.erase(notifications.begin() + i);
        } 
        else {
            ImGui::Begin("Notification", nullptr, ImGuiWindowFlags_NoDecoration |  ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::TextWrapped("%s",notification.text.c_str()); // MEMO3
            ImGui::Separator();
            ImGui::TextWrapped("%s", notification.text.c_str()); //MEMO5
            ImGui::End();
            i++;
        }
    }
}

void style() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 20.0f;
    style.Colors[ImGuiCol_Button] = ImVec4(193.0f/255,60.0f/255,60.0f/255,255.0f/255);
};






ImVec4 hexToImVec4(const std::string& hex) {
    std::string h = hex;
    if (h[0] == '#') h = h.substr(1);
    if (h.length() != 6 && h.length() != 8) return ImVec4(1,1,1,1); // fallback bianco
    unsigned int r, g, b, a = 255;
    std::stringstream ss;
    ss << std::hex << h.substr(0,2);
    ss >> r;
    ss.clear(); ss.str("");
    ss << std::hex << h.substr(2,2);
    ss >> g;
    ss.clear(); ss.str("");
    ss << std::hex << h.substr(4,2);
    ss >> b;
    if (h.length() == 8) {
        ss.clear(); ss.str("");
        ss << std::hex << h.substr(6,2);
        ss >> a;
    }
    return ImVec4(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}

void DrawGlowingRectangle() {
    ImGui::Begin("Glowing Rectangle Window");

    // get current drawlist
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // define size and position
    ImVec2 pos = ImGui::GetCursorPos();
    ImVec2 rect_min = ImVec2(pos.x+50, pos.y+50);
    ImVec2 rect_max = ImVec2(pos.x+150, pos.y+150);

    // define glow and base color
    ImU32 glow_color = IM_COL32(0,150,255,60);
    ImU32 base_color = IM_COL32(0,150,255,255);

    //draw multiple layers of glowing effect with increased thickness
    for( int i = 0; i < 15; i++) {
        float glow_thickness = 10.0f + i * 3.0f;
        draw_list->AddRect(rect_min, rect_max, glow_color, 0.0f, ImDrawFlags_RoundCornersAll, glow_thickness);
    }
    draw_list->AddRectFilled(rect_min,rect_max,base_color);
    ImGui::End();
}

void HardLoad() {
    //simula lavoro lungo o pesante
    std::this_thread::sleep_for(std::chrono::seconds(5));
    //
    //std::thread t(hardLoad);
    //t.detach(); // Il thread lavora in background, non blocca il main thread
    //se invece vuoi apsettare che finisca prima di proseguire  usa t.join()
}

int lavoro_lungo() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 42; // risultato del calcolo
}

void esempio_future() {
    std::future<int> risultato = std::async(std::launch::async, lavoro_lungo);

    // Puoi fare altro mentre il thread lavora...

    // Quando ti serve il risultato:
    int valore = risultato.get(); // attende che il thread finisca
    std::cout << "Risultato: " << valore << std::endl;
}

namespace Math {
    unsigned int fact(unsigned int n) {
        if (n == 1 || n == 0) return 1;
        return n * fact(n-1);
    }
}
std::string ImVec2_to_string(const ImVec2& vector ) {
    std::string out = "";
    out += vector.x;
    return out;
}

bool checkWindowSizeChange(const ImVec2& currentSize, float checkIntervalSeconds) {
    static ImVec2 lastSize = currentSize;
    static auto lastCheckTime = std::chrono::steady_clock::now();

    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastCheckTime);

    if (elapsed.count() >= checkIntervalSeconds * 1000) {
        if (currentSize.x != lastSize.x || currentSize.y != lastSize.y) {
            std::cout << "Window size changed! width: " << currentSize.x 
                        << " height: " << currentSize.y << std::endl;
            lastSize = currentSize;
            lastCheckTime = currentTime;
            return true; // Dimensione cambiata
        }
        lastCheckTime = currentTime;
    }
    return false; // Nessun cambiamento o timer non scaduto
}

// TODO
// ImVec2 CenterPosWindow(ImVec2 imvec2, GLFWwindow* window_ptr) {

// }

/**
 * Trova il numero mancante in un array da 1 a n-1 usando lo xor trick
 */
// int findMissingNumber(uint8_t a[]) {
//     int size = sizeof(a) / 8;
//     int b,c;
//     b ^= c;
// }

Error Rectangle(const ImVec2& pos, const ImVec2& size, color col) {
    
    
    bool bool_rect = true;
    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(size);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, col);
    
    // Crea un ID unico per ogni rettangolo
    element_id++;
    std::string rectangle_id = "rectangle_" + std::to_string(element_id);
    
    ImGui::Begin(rectangle_id.c_str(), &bool_rect, flags_rectangle);
    ImGui::End();
    ImGui::PopStyleColor();
    return Error::OK;
}



void attack(const std::string& target_ip, const int& port, const std::string& attack_type, bool claymore, bool spread, bool network_spread, int err[]) {
    // Inizializza tutte le celle del bitvector a zero
    for (int i = 0; i < 8; ++i) err[i] = 0;
    if (checkIp(target_ip) == Error::FAIL) {
        err[0] = 1; // Errore IP
        throw std::string("Invalid IP address format");
    }

    // Esempio: errore porta
    if (port < 1 || port > 65535) {
        err[1] = 1; // Errore porta
        throw std::string("Invalid port number");
    }

    // Simula successo/fallimento
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);
    bool success = dist(gen);
    time_t timestamp = time(0);
    std::string timestamp_str = time_t_to_string(timestamp);

    save_attack_json(target_ip, port, attack_type, claymore, spread, network_spread, success, timestamp_str);
    db("Attack creation failed");
}

void save_attack_json(const std::string& ip, const int& port, const std::string& attack_type, bool claymore, bool spread, bool network_spread, bool success, std::string timestamp) {
    // Crea un oggetto JSON con i dati dell'attacco
    json attack = {
        {"ip", ip},           // IP bersaglio
        {"port", port},       // Porta bersaglio
        {"type", attack_type},// Tipo di attacco
        {"claymore", claymore}, // Se è previsto malware
        {"spread", spread},
        {"network_spread", network_spread}, // Se si vuole diffondere l'attacco
        {"success", success}, // Se l'attacco ha avuto successo
        {"timestamp", timestamp}    // Timestamp dell'attacco
    };

    // Prova ad aprire il file attacks.json per leggere eventuali attacchi già salvati
    std::ifstream infile(attacks_file_path);
    json attacks;
    if (infile.is_open()) {
        infile >> attacks;      // Carica il contenuto del file nel json attacks
        infile.close();         // Chiudi il file
    }
    // Se il file non contiene un array, inizializza un array vuoto
    if (!attacks.is_array()) attacks = json::array();

    // Aggiungi il nuovo attacco all'array
    attacks.push_back(attack);

    // Scrivi l'array aggiornato nel file attacks.json, con indentazione 4 spazi
    std::ofstream outfile(attacks_file_path);
    outfile << attacks.dump(4);
    outfile.close();            // Chiudi il file
}

Error checkIp(const std::string& ip) {
  std::regex ip_regex(R"((\d{1,3}\.){3}\d{1,3})");
  if (!std::regex_match(ip, ip_regex)) {
      return Error::FAIL;
  }

  // Controlla se l'IP è nella gamma valida
  std::istringstream iss(ip);
  std::string token;
  while (std::getline(iss, token, '.')) {
      int octet = std::stoi(token);
      if (octet < 0 || octet > 255) {
          return Error::FAIL;
      }
  }

  return Error::OK;
}

std::string time_t_to_string(time_t t) {
    char buffer[32];
    struct tm* timeinfo = localtime(&t);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}



std::string get_device_name() {
#ifdef _WIN32
    char computerName[256];
    DWORD size = sizeof(computerName);
    if (GetComputerNameA(computerName, &size)) {
        return std::string(computerName);
    }
    return "Unknown";
#else
    struct utsname buf;
    if (uname(&buf) == 0) {
        return std::string(buf.nodename);
    }
    return "Unknown";
#endif
}

std::string get_os_info() {
#ifdef _WIN32
    return "Windows";
#else
    struct utsname buf;
    if (uname(&buf) == 0) {
        std::stringstream ss;
        ss << buf.sysname << " " << buf.release << " " << buf.version;
        return ss.str();
    }
    return "Unknown";
#endif
}

std::string get_cpu_info() {
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::stringstream ss;
    ss << "Arch: " << sysInfo.wProcessorArchitecture << ", Cores: " << sysInfo.dwNumberOfProcessors;
    return ss.str();
#else
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    std::stringstream ss;
    ss << "Cores: " << nprocs;
    return ss.str();
#endif
}

size_t get_total_ram_mb() {
#ifdef _WIN32
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if (GlobalMemoryStatusEx(&statex)) {
        return static_cast<size_t>(statex.ullTotalPhys / (1024 * 1024));
    }
    return 0;
#else
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return static_cast<size_t>((pages * page_size) / (1024 * 1024));
#endif
}

size_t get_free_ram_mb() {
#ifdef _WIN32
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if (GlobalMemoryStatusEx(&statex)) {
        return static_cast<size_t>(statex.ullAvailPhys / (1024 * 1024));
    }
    return 0;
#else
    long free_pages = sysconf(_SC_AVPHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return static_cast<size_t>((free_pages * page_size) / (1024 * 1024));
#endif
}

SystemInfo get_system_info() {
    SystemInfo info;
    info.deviceName = get_device_name();
    // Split OS info into name and version on Windows
#ifdef _WIN32
    OSVERSIONINFOA osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOA));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    
    #pragma warning(suppress: 4996)
    if (GetVersionExA(&osvi)) {
        info.osName = "Windows";
        std::stringstream ss;
        ss << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << " (Build " << osvi.dwBuildNumber << ")";
        info.osVersion = ss.str();
    } else {
        info.osName = "Windows";
        info.osVersion = "Unknown Version";
    }
#else
    struct utsname buf;
    if (uname(&buf) == 0) {
        info.osName = buf.sysname;
        std::stringstream ss;
        ss << buf.release << " " << buf.version;
        info.osVersion = ss.str();
    } else {
        info.osName = "Unknown";
        info.osVersion = "Unknown";
    }
#endif
    
    info.cpuInfo = get_cpu_info();
    info.totalRamMB = get_total_ram_mb();
    info.freeRamMB = get_free_ram_mb();
    
    return info;
}

bool get_monitor_resolution(int* width, int* height) {
    if (!glfwInit()) {
        return false;
    }
    
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        glfwTerminate();
        return false;
    }
    
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!mode) {
        glfwTerminate();
        return false;
    }
    
    *width = mode->width;
    *height = mode->height;
    
    return true;
}

std::pair<int, int> calculate_window_size(float scale_factor) {
    int monitor_width, monitor_height;
    
    if (!get_monitor_resolution(&monitor_width, &monitor_height)) {
        // Fallback to default values if monitor detection fails
        return std::make_pair(1920, 1080);
    }
    
    // Calcola le dimensioni della finestra come percentuale del monitor
    int window_width = static_cast<int>(monitor_width * scale_factor);
    int window_height = static_cast<int>(monitor_height * scale_factor);
    
    // Assicurati che le dimensioni minime siano rispettate
    window_width = std::max(window_width, 800);
    window_height = std::max(window_height, 600);
    
    return std::make_pair(window_width, window_height);
}

void initialize_window_settings(float scale_factor) {
    std::pair<int, int> window_size = calculate_window_size(scale_factor);
    frame_window_width_setting = window_size.first;
    frame_window_heigth_setting = window_size.second;
    
    db("Dimensioni finestra calcolate dinamicamente:", frame_window_width_setting, "x", frame_window_heigth_setting);
}

void update_imgui_layout(int window_width, int window_height) {
    // Aggiorna le dimensioni globali
    frame_window_width_setting = window_width;
    frame_window_heigth_setting = window_height;
    
    // Forza il ridimensionamento delle finestre ImGui principali
    // Puoi aggiungere qui la logica per ridimensionare finestre specifiche
    
    db("Layout aggiornato per:", window_width, "x", window_height);
}
