#pragma once // serve a evitare che lo stesso header venga incluso più volte
//standard
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

// frame che ospiterà le finestre

extern int frame_window_width_setting;
extern int frame_window_heigth_setting;
extern std::string frame_window_title_setting;
extern GLFWmonitor *frame_window_monitor_setting;
extern GLFWwindow *frame_window_share_setting;

extern ImGuiWindowFlags flags_minimize_and_exit_window;
extern ImGuiWindowFlags flags_login_window;
extern ImGuiWindowFlags flags_my_window;
extern ImGuiWindowFlags flags_demo_window;
extern ImGuiWindowFlags flags_attack_window; 
extern ImGuiWindowFlags flags_window1; 
extern ImGuiWindowFlags flags_menu_bar;
extern ImGuiWindowFlags flags_tab_window; 
extern ImGuiWindowFlags flags_tree; 
extern ImGuiWindowFlags flags_table; 
extern ImGuiWindowFlags flags_settings;
extern ImGuiWindowFlags flags_BottomBar;
extern ImGuiWindowFlags flags_rectangle;
extern ImGuiWindowFlags flags_HelpWindow;
extern ImGuiWindowFlags flags_network;


// Bools
extern bool bool_minimize_and_exit_window;
extern bool bool_demo_window;
extern bool bool_my_window;
extern bool bool_window1;
extern bool bool_login_window;
extern bool bool_attack_window;
extern bool bool_menu_bar;
extern bool bool_tab_window;
extern bool bool_Tree;
extern bool bool_table;
extern bool bool_settings;
extern bool bool_BottomBar;
extern bool bool_HelpWindow;
extern bool bool_network;

// Settings window arrays for dropdown menus
extern const char* languages[5];
extern const char* resolutions[7];
extern const char* antialiasing_options[5];
extern const char* notification_positions[4];

// Impostazioni Window 1 
extern float value1;
extern int value2;
extern std::vector<const char*> items;
extern int current;
//
extern bool esperimenti_window;
extern float my_float;
extern int my_int;

extern const std::string username;
extern const std::string password;

void setAuthenticatedWindows(bool enabled);

extern int element_id;