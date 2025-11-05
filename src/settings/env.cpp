//standard
#include <iostream>
#include <sstream>
#include <thread>
//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
//src
#include "env.h"

const std::string app_name = APP_NAME;
const std::string version = APP_VERSION;
extern const bool DEBUG = true; 
extern const bool DEV = true; 

const std::string attacks_file_path = "data/attacks.json";