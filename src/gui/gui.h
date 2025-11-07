#pragma once
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

namespace Gui {
/**
 * Classe primaria
 * Frame
 */
class FrameWindowManager {
public:
    int frame_window_width; // la larghezza del frame fww
    int frame_window_heigth; // l' altezza del frame fwh
    std::string title; // titolo del frame
    GLFWmonitor *monitor;  // The monitor to use for full screen mode, or NULL for windowed mode.
    GLFWwindow *share;  // The window whose context to share resources with, or NULL to not share resources

    
    FrameWindowManager(int width, int heigth, const std::string& t, GLFWmonitor *mon, GLFWwindow *sha); // costruttore
    ~FrameWindowManager(); // distruttore
    /**
     * Funzione che genera la finestra principale del programma
     * `frame_window` : variabile in cui verrà messo il risultato della funzione
     * glfwCreateWindow()
     * se !main_window ritorna false
     */
    Error Create();
    /**
     * Funzione che ritorna il puntatore GLFWwindow* alla finestra principale
     */
    GLFWwindow* getWindow() const;
    
    /**
     * Getter per le dimensioni aggiornate della finestra
     */
    int getCurrentWidth() const { return frame_window_width; }
    int getCurrentHeight() const { return frame_window_heigth; }
    /**
   * Renderizza la finestra
   * `fwbc` : [frame_window_background_color]: color of the background
   */
    Error RenderFrame(const ImVec4& fwbc);
    void CleanUp();

private:
    GLFWwindow* frame_window = nullptr;
};
/**
 * CLasse primaria
 */
class Window { // inizio class Window
protected:
    std::string title;
    ImVec2 pos;
    ImVec2 size;
    bool isOpen;
    ImGuiWindowFlags flags;
    

public:
    // Modifica il costruttore per accettare anche i flags (con default opzionale)
    Window(const std::string& t, ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f = 0)
        : title(t), pos(p), size(s), isOpen(true), flags(f), window_ptr(win)  {}
        
    // distruttore virtuale che garantisce corretta distruzione nella gerarchia
    virtual ~Window() = default; 
    // Funzione virtuale pura: ogni finestra la implementa
    virtual void Render() = 0; 

    ImVec2 GetPos() {
        return pos; 
    }

    ImVec2 GetSize() {
        return size; 
    }

    void SetPos(const ImVec2& p) {
        pos = p; 
    }
    void SetSize(const ImVec2& s) {
        size = s; 
    }
    void SetTitle(const std::string& t) {
        title = t; 
    }
    bool* GetOpenPtr() {
        return &isOpen;
    }
    inline float Area() {
        return size.x * size.y;
    }

    struct WindowVertices {
        ImVec2 p1;
        ImVec2 p2;
        ImVec2 p3;
        ImVec2 p4;

    };

    WindowVertices GetVertices() const;

    
protected:
    GLFWwindow* window_ptr;
}; // fine class window

//^ ///////////////////////////////////////////////////////// Login Window ///////////////////////////////////////////////////////////
/**
 * Finestra di Login
 */
class LoginWindow : public Window {
private:
    char username[64] = "";
    char password[64] = "";
    bool login_success = false;
    
    
    
    public:
    LoginWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f); // costruttore della classe
    void Render() override;
    bool logged = false;
    void isLogged();
    
}; // fine classe loginWindow



//& ShowMyWindow /////////////////////////////////////////////////////////////////////////////////////////////////////

class ShowMyWindow : public Window {
public:
    ShowMyWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f); //il costruttore deve avere lo stesso nome della classe
    unsigned int count = 0; // counter del bottone clickMe
    void Render() override;
}; // fine class ShowMyWindow



class SettingsWindow : public Window {
private:
    // General settings
    bool dark_theme = true;
    float global_scale = 1.0f;
    int language_index = 0;
    
    // Display settings
    bool vsync = true;
    bool fullscreen = false;
    int resolution_index = 0;
    float brightness = 1.0f;
    float contrast = 1.0f;
    
    // Audio settings
    float master_volume = 0.8f;
    float music_volume = 0.7f;
    float effects_volume = 0.9f;
    bool mute = false;
    
    // Advanced settings
    int antialiasing_index = 0;
    bool motion_blur = false;
    float shadow_quality = 0.5f;
    bool advanced_lighting = true;
    
    // Notification settings
    bool show_notifications = true;
    float notification_duration = 3.0f;
    int notification_position_index = 0;
    
    // Save settings function
    void SaveSettings();
    
    // Reset settings function
    void ResetToDefaults();
    
    void ApplyDisplaySettings();

    // Current active tab
    int active_tab = 0;
    
    // Altezza della barra dei menu per il posizionamento
    float menubar_height = 19.0f; // Altezza standard della barra dei menu di ImGui 

public:
    SettingsWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f);
    void Render() override;
};
//! ///////////////////////////////////////////Minimize and exit window ///////////////////////////////////////////////
class MinimizeAndExitWindow : public Window {
public:
    MinimizeAndExitWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f); // costruttore
    void Render() override;
    float bar_height = 30.0f;
    
}; // fine classe minimize and exit window

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class showDemoWindow : public Window {
public:
    showDemoWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f);//costruttore
    void Render() override;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AttackWindow : public Window {
public:
    AttackWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f); // costruttore
    void Render() override;
    void startScan(const std::string& ip, int port);
    void RenderScanUI();
private:
    std::atomic<bool> scan_in_progress = false;
    std::atomic<float> scan_progress = 0.0f;
    std::vector<std::string> scan_results;

};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Window1 : public Window {
public:
    Window1(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f); // costruttore
    void Render() override;
};

class MenuBar : public Window {
public: 
    MenuBar(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f); // A quanto pare menubar ignora pos e size e si posiziona il alto a sinistra della finestra madre. Sempre
    /**
     * Render della menubar
     */
    void Render() override;
    bool checked;
    

};

class TabWindow : public Window {
private:
    bool tab1_open = true;
    bool tab2_open = true;
    bool tab3_open = true;
    bool tab4_open = true;

public: 
    TabWindow(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f);
    

    void Render() override;
};

class Tree : public Window {
public:
    Tree(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f);

    void Render() override;
};

class Table : public Window {
public:
    Table(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f);

    void Render() override;
};

class BottomBar : public Window {
public: 
    float height = 35.0f;
    BottomBar(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f);
    void Render() override;
};

class HelpWindow : public Window {
private:
    float menubar_height = 19.0f; // Altezza standard della barra dei menu di ImGui //TODO da spostare in una variabile esterna
public:
    HelpWindow(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f);
    void Render() override;
};


;} // fine namespace Gui
