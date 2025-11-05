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
  SettingsWindow::SettingsWindow(ImVec2 p, ImVec2 s, GLFWwindow* win, ImGuiWindowFlags f)
    : Window("Settings", p, s, win, f) {
        // Constructor implementation
    }

    void SettingsWindow::SaveSettings() {
        // Here you would implement saving settings to a file or database
        ApplyDisplaySettings();
        set_color_theme(color_theme_set);
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
    int width,height;
    glfwGetFramebufferSize(window_ptr, &width, &height);
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
                    ImGui::Text("Color Theme");
                    if (ImGui::RadioButton("Default", color_theme_set==ColorTheme::Blu)) color_theme_set=ColorTheme::Blu;
                    if (ImGui::RadioButton("Red",     color_theme_set==ColorTheme::Rosso)) color_theme_set=ColorTheme::Rosso;
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
}