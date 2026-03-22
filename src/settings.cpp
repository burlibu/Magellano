//In this file no functions are allowed -> create them in functions.cpp

//standards
#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
//src
#include "settings.h"

// Frame_window
GLFWwindow* frame_window = nullptr;
int frame_window_width_setting = 1920;
int frame_window_heigth_setting = 1080;
std::string frame_window_title_setting = "Alberus Project";
GLFWmonitor *frame_window_monitor_setting = nullptr;
GLFWwindow *frame_window_share_setting = nullptr;

bool bool_minimize_and_exit_window      = false;
bool bool_demo_window                   = false;
bool bool_my_window                     = false;
bool bool_window1                       = false;
bool bool_login_window                  = false;
bool bool_attack_window            = true;
bool bool_menu_bar                      = true;
bool bool_tab_window                    = true;
bool bool_Tree                          = true;
bool bool_table                         = true;
bool bool_settings                      = false;
bool bool_BottomBar                     = true;
bool bool_HelpWindow                    = false;
bool bool_network                       = false;
bool bool_changelog                     = false;

const std::array<TopPanelToggleEntry, 4> top_panel_toggle_entries = {{
    {"Settings", &bool_settings, ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ImVec4(0.0f, 0.9f, 0.0f, 1.0f), ImVec4(0.0f, 0.7f, 0.0f, 1.0f)},
    {"Help", &bool_HelpWindow, ImVec4(0.35f, 0.35f, 0.35f, 1.0f), ImVec4(0.45f, 0.45f, 0.45f, 1.0f), ImVec4(0.25f, 0.25f, 0.25f, 1.0f)},
    {"Network", &bool_network, ImVec4(0.0f, 0.4f, 0.8f, 1.0f), ImVec4(0.0f, 0.5f, 0.9f, 1.0f), ImVec4(0.0f, 0.3f, 0.7f, 1.0f)},
    {"Changelog", &bool_changelog, ImVec4(0.75f, 0.55f, 0.10f, 1.0f), ImVec4(0.85f, 0.65f, 0.15f, 1.0f), ImVec4(0.65f, 0.45f, 0.05f, 1.0f)}
}};

const std::array<WindowToggleEntry, 6> authenticated_window_entries = {{
    {"Main", &bool_my_window},
    {"Window1", &bool_window1},
    {"Demo", &bool_demo_window},
    {"Attack", &bool_attack_window},
    {"Network", &bool_network},
    {"Changelog", &bool_changelog}
}};

ImGuiWindowFlags flags_minimize_and_exit_window =
    ImGuiWindowFlags_NoTitleBar 
    | ImGuiWindowFlags_NoResize 
    | ImGuiWindowFlags_NoMove 
    | ImGuiWindowFlags_NoScrollbar
    | ImGuiWindowFlags_NoSavedSettings 
    | ImGuiWindowFlags_NoCollapse 
    | ImGuiWindowFlags_NoBringToFrontOnFocus
    | ImGuiWindowFlags_NoFocusOnAppearing 
    | ImGuiWindowFlags_NoNav;
  
ImGuiWindowFlags flags_login_window = 0; 

ImGuiWindowFlags flags_my_window = 0; 

ImGuiWindowFlags flags_demo_window = 0;

ImGuiWindowFlags flags_attack_window = 
    ImGuiWindowFlags_NoSavedSettings
    | ImGuiWindowFlags_NoCollapse
    | ImGuiWindowFlags_NoResize 
    | ImGuiWindowFlags_NoMove  
    ; 
ImGuiWindowFlags flags_window1 = 0; 
ImGuiWindowFlags flags_menu_bar = 0; 
ImGuiWindowFlags flags_tab_window = 
    ImGuiWindowFlags_NoSavedSettings 
    | ImGuiWindowFlags_NoCollapse 
    | ImGuiWindowFlags_NoResize 
    | ImGuiWindowFlags_NoMove 
    ;

ImGuiWindowFlags flags_tree = 
    ImGuiWindowFlags_NoSavedSettings
    | ImGuiWindowFlags_NoCollapse 
    | ImGuiWindowFlags_NoResize 
    | ImGuiWindowFlags_NoMove 
    | ImGuiWindowFlags_NoBringToFrontOnFocus
    | ImGuiWindowFlags_NoFocusOnAppearing 
    ;
ImGuiWindowFlags flags_table = 
    ImGuiWindowFlags_NoSavedSettings
    | ImGuiWindowFlags_NoCollapse 
    | ImGuiWindowFlags_NoResize 
    | ImGuiWindowFlags_NoMove 
    | ImGuiWindowFlags_NoBringToFrontOnFocus
    | ImGuiWindowFlags_NoFocusOnAppearing 
    ;
// ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable

ImGuiWindowFlags flags_settings =  
      ImGuiWindowFlags_NoResize 
    | ImGuiWindowFlags_NoMove 
    | ImGuiWindowFlags_NoScrollbar
    | ImGuiWindowFlags_NoSavedSettings 
    | ImGuiWindowFlags_NoCollapse 
    // | ImGuiWindowFlags_NoBringToFrontOnFocus
    | ImGuiWindowFlags_NoFocusOnAppearing 
    ;

ImGuiWindowFlags flags_BottomBar = 
    ImGuiWindowFlags_NoTitleBar 
    | ImGuiWindowFlags_NoResize 
    | ImGuiWindowFlags_NoMove 
    | ImGuiWindowFlags_NoScrollbar
    | ImGuiWindowFlags_NoSavedSettings 
    | ImGuiWindowFlags_NoCollapse 
    | ImGuiWindowFlags_NoBringToFrontOnFocus
    | ImGuiWindowFlags_NoFocusOnAppearing 
    | ImGuiWindowFlags_NoNav
    ;

ImGuiWindowFlags flags_rectangle = 
    ImGuiWindowFlags_NoTitleBar 
    | ImGuiWindowFlags_NoResize 
    | ImGuiWindowFlags_NoMove 
    | ImGuiWindowFlags_NoScrollbar
    | ImGuiWindowFlags_NoSavedSettings 
    | ImGuiWindowFlags_NoCollapse 
    | ImGuiWindowFlags_NoBringToFrontOnFocus
    | ImGuiWindowFlags_NoFocusOnAppearing 
    | ImGuiWindowFlags_NoNav
    ;

ImGuiWindowFlags flags_HelpWindow = 
      ImGuiWindowFlags_NoResize 
    | ImGuiWindowFlags_NoMove 
    | ImGuiWindowFlags_NoScrollbar
    | ImGuiWindowFlags_NoSavedSettings 
    | ImGuiWindowFlags_NoCollapse 
    // | ImGuiWindowFlags_NoBringToFrontOnFocus
    | ImGuiWindowFlags_NoFocusOnAppearing 
    | ImGuiWindowFlags_NoNav
    ;

ImGuiWindowFlags flags_network =
            ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoSavedSettings
        ;

ImGuiWindowFlags flags_changelog =
            ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoSavedSettings
        ;
    
// Settings window arrays for dropdown menus
const char* languages[] = { "English", "Italian", "Spanish", "French", "German" };
const char* resolutions[] = { "1280x720", "1366x768", "1600x900", "1920x968", "1920x1080", "2550x1400", "2560x1440", "3840x2160" };
const char* antialiasing_options[] = { "Off", "FXAA", "MSAA 2x", "MSAA 4x", "MSAA 8x" };
const char* notification_positions[] = { "Top Right", "Top Left", "Bottom Right", "Bottom Left" };




// Impostazioni Window 1 
float value1 =  0.1f;
int value2 = 1;
std::vector<const char*> items = {"item1", "item2", "item3"};
int current = 0;
//
bool esperimenti_window = false;
float my_float = 0.0f;
int my_int = 0;


const std::string username = "admin";
const std::string password = "password";

int element_id = 0;