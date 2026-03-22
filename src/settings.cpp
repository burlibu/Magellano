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
const char* resolutions[] = { "1280x720", "1366x768", "1600x900", "1920x1080","2550x1400", "2560x1440", "3840x2160" };
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

void setAuthenticatedWindows(bool enabled) {
    bool_my_window = enabled;
    bool_window1 = enabled;
    bool_demo_window = enabled;
    bool_attack_window = enabled;
    bool_network = enabled;
    bool_changelog = enabled;
}

int element_id = 0;