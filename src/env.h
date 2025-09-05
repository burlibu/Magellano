#pragma once
//standard
#include <iostream>
#include <sstream>
#include <thread>
//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>



#define APP_VERSION "0.0.5"
extern const std::string version;
extern const bool DEBUG; 

extern const std::string attacks_file_path;