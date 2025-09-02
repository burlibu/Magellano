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
#include "env.h"
#include "settings.h"
#include "functions.h"
#include "custom_colors.h"
#include "gui.h"

namespace Gui {
    // Frame Window ////////////////////////////////////////////////////////////////////////////////////////////////////
    FrameWindowManager::FrameWindowManager(int width, int height, const std::string& t, GLFWmonitor *mon, GLFWwindow *sha) 
        : frame_window_width(width), frame_window_heigth(height), title(t.c_str()), monitor(mon), share(sha), frame_window(nullptr){
        // attenzione: qui perdi la proprietà di std::string ! -> ->  ^   
    }
    FrameWindowManager::~FrameWindowManager() {
        if (frame_window) {
            glfwDestroyWindow(frame_window);
        }
    }

    Error FrameWindowManager::Create() {
        frame_window = glfwCreateWindow(frame_window_width, frame_window_heigth, title.c_str(), monitor, share);
        if (!frame_window) return Error::FAIL;
        return Error::OK;
    }
    
    GLFWwindow* FrameWindowManager::getWindow() const {
        return frame_window;
    }


    /**
     * Renderizza la finestra
     * `fwbc` : [frame_window_background_color]: color of the background
     */
    Error FrameWindowManager::RenderFrame(const ImVec4& fwbc) {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(frame_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(fwbc.x, fwbc.y, fwbc.z, fwbc.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(frame_window);
    return Error::OK;
    }

    void FrameWindowManager::CleanUp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (frame_window) {
        glfwDestroyWindow(frame_window);
        frame_window = nullptr;
        }
    glfwTerminate();
    }

    Window::WindowVertices Window::GetVertices () const{
        Window::WindowVertices verts;
        verts.p1 = pos;
        verts.p2 = ImVec2(pos.x + size.x, pos.y);
        verts.p3 = ImVec2(pos.x + size.x, pos.y + size.y);
        verts.p4 = ImVec2(pos.x, pos.y + size.y);
        return verts;
    }




    //^ LoginWindow ////////////////////////////////////////////////////////////////////////////////////////////////////
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
    //^ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    //& ShowMyWindow //////////////////////////////////////////////////////////////////////////////////////////////////////
    ShowMyWindow::ShowMyWindow(ImVec2 position, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f) 
        : Window("My window", position, size, win, f) {}
    /**
     *  Parameters:
     * 
     * `const ImVec2& cursorpos` posizione del cursore dove sarà l' angolo alto sinistro della schermata
     * `const ImVec2& window_size` larghezza e altezza in un imvec2
     * Attenzione: chiama checkWindowSize se DEBUG è attivo
     */
    void ShowMyWindow::Render() {
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::Begin("MyWindow", &isOpen);
        ImGui::Text("Benvenuto nel mio progetto ImGui!");
        ImGui::Separator();
        
        ImGui::SliderFloat("Float Value", &my_float, 0.0f, 1.0f);
        ImGui::SliderInt("Int Value", &my_int, 0, 100);
        
        if (ImGui::Button("Click Me!")) {
            std::cout << "Bottone cliccato!" << std::endl;
            count++;
        }
        
        ImGui::SameLine();
        ImGui::Text("Counter: %d", count);
        
        ImGui::End();

        if (DEBUG) {
        
        }
    }
    //& //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //! /////////////////////////////////////Minimize and exit window ////////////////////////////////////////////////////
    MinimizeAndExitWindow::MinimizeAndExitWindow(ImVec2 position, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f) 
        : Window("MinimizeAndExitWindow", position, size, win, f) {};

    void MinimizeAndExitWindow::Render() {
        // Aggiorna la larghezza della barra in base alla finestra principale
        int width, height;
        glfwGetFramebufferSize(window_ptr, &width, &height);
        size.x = width; // aggiorna la larghezza della barra
        // Mantieni la posizione sempre in alto a sinistra
        pos = ImVec2(0, 0);

        ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        ImGui::Begin("minimize_and_exit_window_id", &isOpen, flags_minimize_and_exit_window);
        //
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4)); // padding interno dei bottoni (il blu)
        // Posiziona i bottoni a destra
        float windowWidth = ImGui::GetWindowWidth();
        ImGui::SetCursorPosX(windowWidth - ImGui::CalcTextSize("- X").x - 30);
        // Bottone Minimize
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, arancione); // hover arancione
        ImGui::PushStyleColor(ImGuiCol_Text, bianco); // testo bianco
        if (ImGui::Button("-")) {
            glfwHideWindow(window_ptr); 
        }
        ImGui::PopStyleColor(2);
        ImGui::SameLine();

        // Bottone Exit
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, rosso); // hover rosso
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1)); // testo bianco
        if (ImGui::Button("X")) {
            exit(0);
        }
        checkWindowSizeChange(size);
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
        ImGui::End();
    } // fine MinimizeAndExitWindow::Render()
    ////////////////////////////////////////////////////Show Demo Window///////////////////////////////////////////////////////
    showDemoWindow::showDemoWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f) 
    : Window("Demo Window", p, s, win, f){ // costruttore con lista di inizializzazione

    };
    void showDemoWindow::Render() {
        ImGui::ShowDemoWindow(&bool_demo_window);
    };

    //////////////////////////////////////////////////Esperimenti Window ////////////////////////////////////////////////////
    AttackWindow::AttackWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window("Attack Window", p, s, win, f) {

    }
    void AttackWindow::Render() {
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::Begin("Attack", &bool_esperimenti_window, flags_esperimenti_window);

        // Input text
        static char text[128] = "";
        ImGui::InputText("Target Ip", text, IM_ARRAYSIZE(text));

        // Checkbox
        static bool malware_checkbox = false;
        ImGui::Checkbox("Deploy Malware", &malware_checkbox);

        static bool spread_checkbox = false;
        ImGui::Checkbox("Spread to nearby devices", &spread_checkbox);

        // Radio buttons
        static int radio = 0;
        ImGui::RadioButton("Ddos Attack : deny connection and services to and from Rtarget by flooding it with junk traffic.", &radio, 0);
        ImGui::RadioButton("Reverse Shell : obtain access to the target operating system in order to gather information or modify it.", &radio, 1);
        ImGui::RadioButton("Malware injection : insert in the target's device a malicious code capable of incapacitating defenses or even destroy it.", &radio, 2);
        

        // Color picker
        static float color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
        ImGui::ColorEdit4("Colore", color);

        ImGui::End();
    }

    //////////////////////////////////////Window 1 /////////////////////////////////////////////////////////////////
    Window1::Window1(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window("Window1", p, s, win, f) {

    }

    void Window1::Render() {
    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
    ImGui::Begin("Window1", &bool_window1, flags_window1);

    if (ImGui::Button("My button", ImVec2(100,100))) {
        // tutto quello in questo if verrà eseguito quando il bottone sarà premuto
        ImGui::Text("You pressed the button");
    }
    bool checkBoxValue = true;
    ImGui::Checkbox("CheckBox", &checkBoxValue);

    ImGui::SliderFloat("My Float Slider", &value1, 0.0f, 100.0f);

    ImGui::Combo("##id1", &current, items.data(), items.size());
    ImGui::SetCursorPosX(ImGui::GetWindowWidth()/2 - ImGui::CalcTextSize("This is some text").x/2);
    ImGui::Text("This is some text");
    ImGui::End();
    };

    //////////////////////////////////////////////// MENU BAR ///////////////////////////////////////////////////////////////
    MenuBar::MenuBar(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window("MenuBar", p, s, win, f), checked(false) {

    }
    void MenuBar::Render() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if(ImGui::MenuItem("New")) {

                }
                if(ImGui::MenuItem("Open")) {
                    
                }
                if(ImGui::MenuItem("Save")) {
                    
                }
                ImGui::EndMenu();
            }
            
            // Cambia lo sfondo della voce Settings quando la finestra è aperta
            bool pushed_styles = false;
            if (bool_settings) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.0f, 0.9f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));
                pushed_styles = true;
            }
            
            if (ImGui::MenuItem("Settings")) {
                // Alterna lo stato della finestra settings
                bool_settings = !bool_settings;
            }
            
            // Ripristina lo stile originale se era stato modificato
            if (pushed_styles) {
                ImGui::PopStyleColor(4); // Pop all 4 style colors that were pushed
            }
            ImGui::SameLine();
            if (ImGui::MenuItem("Help")) {
                bool_HelpWindow = !bool_HelpWindow;
            }
            ImGui::EndMainMenuBar();
        }
    };

    TabWindow::TabWindow(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, pos, size, win, f) {}

    void TabWindow::Render(){
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        if (ImGui::Begin(title.c_str(), nullptr, flags_tab_window)) {
            if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_Reorderable)) {
                // Tab fisso (non chiudibile)
                if (ImGui::BeginTabItem("Generale")) {
                    ImGui::Text("Contenuto tab generale");
                    if (ImGui::Button("Azione 1")) {
                        // Azione del bottone
                    }
                    ImGui::EndTabItem();
                }

                // Tab chiudibili
                if (tab1_open && ImGui::BeginTabItem("Tab 1", &tab1_open)) {
                    ImGui::Text("Contenuto tab 1");
                    ImGui::EndTabItem();
                }

                if (tab2_open && ImGui::BeginTabItem("Tab 2", &tab2_open, ImGuiTabItemFlags_UnsavedDocument)) {
                    ImGui::Text("Contenuto tab 2 (non salvato)");
                    ImGui::EndTabItem();
                }

                if (tab3_open && ImGui::BeginTabItem("Tab 3", &tab3_open)) {
                    ImGui::Text("Contenuto tab 3");
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }

    Tree::Tree(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, pos, size, win, f) {}

    void Tree::Render() {
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::Begin(title.c_str(), nullptr, flags_tree);
        if (ImGui::TreeNode("Nodo Principale")) {
            ImGui::Text("Contenuto del nodo");
            if (ImGui::TreeNode("Elenco Progetti")) {
                ImGui::Text("[Cariddi #1] Alberus");
                ImGui::Text("[Cariddi #2] Valkyrie");
                ImGui::Text("[Cariddi #3] Remini");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Sotto-nodo 2")) {
                ImGui::Text("Contenuto del sotto-nodo 2");
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        ImGui::End();

    }

    Table::Table(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, pos, size, win, f) {}

    void Table::Render() {
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::Begin(title.c_str(), nullptr, flags_table);
        if (ImGui::BeginTable("TabellaAvanzata", 3, flags_table)) {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn("Nome");
            ImGui::TableSetupColumn("Valore");
            ImGui::TableHeadersRow();
        
            for (int row = 0; row < 5; ++row) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%d", row);
                ImGui::TableSetColumnIndex(1); ImGui::Text("Elemento %d", row);
                ImGui::TableSetColumnIndex(2); ImGui::Text("%.2f", row * 1.5f);
            }
        
            ImGui::EndTable();
        }
        ImGui::End();
    }

    SettingsWindow::SettingsWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window("Settings", p, s, win, f) {
        // Constructor implementation
    }

    void SettingsWindow::SaveSettings() {
        // Here you would implement saving settings to a file or database
        ApplyDisplaySettings();

        showNotification("Settings", "Settings saved successfully!", 3000);
    }

    void SettingsWindow::ResetToDefaults() {
        // Reset all settings to their default values
        dark_theme = true;
        global_scale = 1.0f;
        language_index = 0;
        
        vsync = true;
        fullscreen = false;
        resolution_index = 0;
        brightness = 1.0f;
        contrast = 1.0f;
        
        master_volume = 0.8f;
        music_volume = 0.7f;
        effects_volume = 0.9f;
        mute = false;
        
        antialiasing_index = 0;
        motion_blur = false;
        shadow_quality = 0.5f;
        advanced_lighting = true;
        
        show_notifications = true;
        notification_duration = 3.0f;
        notification_position_index = 0;
        
        showNotification("Settings", "Settings reset to defaults!", 3000);
    }

    void SettingsWindow::ApplyDisplaySettings() {
    // Ottieni le dimensioni dal dropdown delle risoluzioni
    int width, height;
    switch(resolution_index) {
        case 0: width = 1280; height = 720; break;
        case 1: width = 1366; height = 768; break;
        case 2: width = 1600; height = 900; break;
        case 3: width = 1920; height = 1080; break;
        case 4: width = 2550; height = 1400; break;
        case 5: width = 2560; height = 1440; break;
        case 6: width = 3840; height = 2160; break;
        default: width = 1920; height = 1080; break;
    }
    
    if (fullscreen) {
        // Modalità fullscreen
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window_ptr, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        // Modalità finestra - sottrai spazio per bordi e barra applicazioni
        int window_width = width;
        int window_height = height - 60; // Sottrai spazio per barra applicazioni
        
        // Centra la finestra
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        int xpos = (mode->width - window_width) / 2;
        int ypos = (mode->height - window_height) / 2;
        
        glfwSetWindowMonitor(window_ptr, nullptr, xpos, ypos, window_width, window_height, 0);
    }
    
    // Applica V-Sync
    glfwSwapInterval(vsync ? 1 : 0);
}

    void SettingsWindow::Render() {
        // Aggiorna le dimensioni della finestra in base al frame principale
        int current_width, current_height;
        glfwGetFramebufferSize(window_ptr, &current_width, &current_height);
        
        // Calcola la posizione e dimensione in base al frame attuale
        ImVec2 current_pos = ImVec2(0, menubar_height);
        ImVec2 current_size = ImVec2(current_width, current_height - menubar_height);
        
        // Imposta la posizione e dimensione aggiornate
        ImGui::SetNextWindowPos(current_pos);
        ImGui::SetNextWindowSize(current_size);
        
        if (ImGui::Begin(title.c_str(), NULL, flags_settings)) {
            // Apply custom style for settings window
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 6));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
            
            // Create tabs for different setting categories
            if (ImGui::BeginTabBar("SettingsTabs", ImGuiTabBarFlags_None)) {
                // General Tab
                if (ImGui::BeginTabItem("General")) {
                    active_tab = 0;
                    ImGui::Spacing();
                    
                    ImGui::Text("Appearance");
                    ImGui::Separator();
                    ImGui::Checkbox("Dark Theme", &dark_theme);
                    ImGui::SliderFloat("UI Scale", &global_scale, 0.5f, 2.0f, "%.1f");
                    ImGui::Combo("Language", &language_index, languages, IM_ARRAYSIZE(languages));
                    
                    ImGui::Spacing();
                    ImGui::Spacing();
                    
                    ImGui::EndTabItem();
                }
                
                // Display Tab
                if (ImGui::BeginTabItem("Display")) {
                    active_tab = 1;
                    ImGui::Spacing();
                    
                    ImGui::Text("Screen Settings");
                    ImGui::Separator();
                    ImGui::Checkbox("V-Sync", &vsync);
                    ImGui::Checkbox("Fullscreen", &fullscreen);
                    ImGui::Combo("Resolution", &resolution_index, resolutions, IM_ARRAYSIZE(resolutions));
                    
                    ImGui::Spacing();
                    ImGui::Spacing();
                    
                    ImGui::Text("Image Adjustments");
                    ImGui::Separator();
                    ImGui::SliderFloat("Brightness", &brightness, 0.0f, 2.0f, "%.1f");
                    ImGui::SliderFloat("Contrast", &contrast, 0.0f, 2.0f, "%.1f");
                    
                    ImGui::EndTabItem();
                }
                
                // Audio Tab
                if (ImGui::BeginTabItem("Audio")) {
                    active_tab = 2;
                    ImGui::Spacing();
                    
                    ImGui::Text("Volume Controls");
                    ImGui::Separator();
                    ImGui::Checkbox("Mute All", &mute);
                    
                    // Disable sliders if muted
                    if (mute) {
                        ImGui::BeginDisabled();
                    }
                    
                    ImGui::SliderFloat("Master Volume", &master_volume, 0.0f, 1.0f, "%.1f");
                    ImGui::SliderFloat("Music Volume", &music_volume, 0.0f, 1.0f, "%.1f");
                    ImGui::SliderFloat("Effects Volume", &effects_volume, 0.0f, 1.0f, "%.1f");
                    
                    if (mute) {
                        ImGui::EndDisabled();
                    }
                    
                    ImGui::EndTabItem();
                }
                
                // Advanced Tab
                if (ImGui::BeginTabItem("Advanced")) {
                    active_tab = 3;
                    ImGui::Spacing();
                    
                    ImGui::Text("Graphics Quality");
                    ImGui::Separator();
                    ImGui::Combo("Anti-aliasing", &antialiasing_index, antialiasing_options, IM_ARRAYSIZE(antialiasing_options));
                    ImGui::Checkbox("Motion Blur", &motion_blur);
                    ImGui::SliderFloat("Shadow Quality", &shadow_quality, 0.0f, 1.0f, "%.1f");
                    ImGui::Checkbox("Advanced Lighting", &advanced_lighting);
                    
                    ImGui::Spacing();
                    ImGui::Spacing();
                    
                    ImGui::EndTabItem();
                }
                
                // Notifications Tab
                if (ImGui::BeginTabItem("Notifications")) {
                    active_tab = 4;
                    ImGui::Spacing();
                    
                    ImGui::Text("Notification Settings");
                    ImGui::Separator();
                    ImGui::Checkbox("Show Notifications", &show_notifications);
                    
                    // Disable other controls if notifications are off
                    if (!show_notifications) {
                        ImGui::BeginDisabled();
                    }
                    
                    ImGui::SliderFloat("Duration (seconds)", &notification_duration, 1.0f, 10.0f, "%.1f");
                    ImGui::Combo("Position", &notification_position_index, notification_positions, IM_ARRAYSIZE(notification_positions));
                    
                    if (!show_notifications) {
                        ImGui::EndDisabled();
                    }
                    
                    ImGui::Spacing();
                    ImGui::Spacing();
                    
                    // Test notification button
                    if (ImGui::Button("Test Notification", ImVec2(150, 30))) {
                        showNotification("Test", "This is a test notification", static_cast<int>(notification_duration * 1000));
                    }
                    
                    ImGui::EndTabItem();
                }
                
                ImGui::EndTabBar();
            }
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            
            // Action buttons at the bottom
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 480);
            if (ImGui::Button("Apply", ImVec2(100, 30))) {
                ApplyDisplaySettings();
                showNotification("Settings", "Display settings applied!", 2000);
            }
            ImGui::SameLine();


            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 360);
            if (ImGui::Button("Reset to Defaults", ImVec2(150, 30))) {
                ImGui::OpenPopup("Reset Settings");
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Save Settings", ImVec2(150, 30))) {
                SaveSettings();
            }
            
            // Confirmation popup for reset
            if (ImGui::BeginPopupModal("Reset Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Are you sure you want to reset all settings to default values?\nThis action cannot be undone.");
                ImGui::Separator();
                
                if (ImGui::Button("Yes", ImVec2(120, 0))) {
                    ResetToDefaults();
                    ImGui::CloseCurrentPopup();
                }
                
                ImGui::SameLine();
                
                if (ImGui::Button("No", ImVec2(120, 0))) {
                    ImGui::CloseCurrentPopup();
                }
                
                ImGui::EndPopup();
            }
            
            ImGui::PopStyleVar(2); // Pop the style variables we pushed earlier
        }
        
        ImGui::End();
    }

    BottomBar::BottomBar(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, pos, size, win, f) {
        
    }

    void BottomBar::Render() {
        int frame_current_width, frame_current_height;
        glfwGetFramebufferSize(window_ptr, &frame_current_width, &frame_current_height);
        pos = ImVec2(0, frame_current_height - height);
        size = ImVec2(frame_current_width, height);
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, blu_oscuro);
        ImGui::Begin(title.c_str(), &bool_BottomBar , flags_BottomBar);
        ImGui::PushStyleColor(ImGuiCol_Text, bianco);
        ImGui::SetCursorPos(ImVec2(size.x - 120, size.y - 25));
        std::string version_text = "version: " + version;
        ImGui::TextUnformatted(version_text.c_str());
        ImGui::PopStyleColor();
        ImGui::End();
        ImGui::PopStyleColor();
    }

    HelpWindow::HelpWindow(const std::string& title, ImVec2 pos, ImVec2 size, GLFWwindow* win, ImGuiWindowFlags f)
    : Window(title, pos, size, win, f) {
    }

    void HelpWindow::Render() {
        // Aggiorna le dimensioni della finestra in base al frame principale
        int current_width, current_height;
        glfwGetFramebufferSize(window_ptr, &current_width, &current_height);

        // Calcola la posizione e dimensione in base al frame attuale
        ImVec2 current_pos = ImVec2(0, menubar_height);
        ImVec2 current_size = ImVec2(current_width, current_height - menubar_height);
        
        // Imposta la posizione e dimensione aggiornate
        ImGui::SetNextWindowPos(current_pos);
        ImGui::SetNextWindowSize(current_size);

        if (ImGui::Begin(title.c_str(), NULL, flags_HelpWindow)) {
            // Apply custom style for settings window
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 6));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
            
            // Create tabs for different setting categories
            if (ImGui::BeginTabBar("SettingsTabs", ImGuiTabBarFlags_None)) {
                // General Tab
                if (ImGui::BeginTabItem("General")) {
                    
                    
                    ImGui::EndTabItem();
                }
                
                // Display Tab
                if (ImGui::BeginTabItem("Display")) {
                    
                    
                    ImGui::EndTabItem();
                }
                
                // Audio Tab
                if (ImGui::BeginTabItem("Audio")) {
                    
                    
                    ImGui::EndTabItem();
                }
                
                // Advanced Tab
                if (ImGui::BeginTabItem("Advanced")) {
                    
                    
                    ImGui::EndTabItem();
                }
                
                // Notifications Tab
                if (ImGui::BeginTabItem("Notifications")) {
                    
                    
                    ImGui::EndTabItem();
                }
                
                ImGui::EndTabBar();
            }
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            
            // Action buttons at the bottom
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 360);
            if (ImGui::Button("Reset to Defaults", ImVec2(150, 30))) {
                ImGui::OpenPopup("Reset Settings");
            }
            
            ImGui::SameLine();
            
            ImGui::PopStyleVar(2); // Pop the style variables we pushed earlier
        }

        ImGui::End();

    }

} // fine namespace gui