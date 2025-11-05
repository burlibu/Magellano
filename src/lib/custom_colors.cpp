//standard
#include <iostream>
//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
//src
#include "custom_colors.h"
#include "functions.h"



const color rosso = {1.0f, 0.0f, 0.0f, 1.0f};
const color arancione = {1.0f, 0.5f, 0.0f, 1.0f};
const color verde_giada = hexToImVec4("70e000"); //#70e000
const color ciano = hexToImVec4("124559"); //#124559
const color crema = hexToImVec4("fdf0d5"); //#fdf0d5
const color blu = hexToImVec4("003566"); //#003566
const color bianco = ImVec4(1,1,1,1); //#ffffff
const color nero = ImVec4(0,0,0,1); //#000000
const color rosa = hexToImVec4("ffc8dd"); //#ffc8dd
const color viola = hexToImVec4("BE00FF"); //#BE00FF
const color verde = hexToImVec4("00FF00"); //#00FF00
const color blu_chiaro = hexToImVec4("00CFFF"); //#00CFFF
const color arancione_chiaro = hexToImVec4("FFA500"); //#FFA500
const color arancione_oscuro = hexToImVec4("FF7F00"); //#FF7F00
const color blu_oscuro = hexToImVec4("003566"); //#003566
const color rosa_chiaro = hexToImVec4("FFC8DD"); //#FFC8DD
const color rosa_oscuro = hexToImVec4("FFA500"); //#FFA500
const color viola_chiaro = hexToImVec4("BE00FF"); //#BE00FF
const color viola_oscuro = hexToImVec4("800080"); //#800080
const color verde_chiaro = hexToImVec4("00FF00"); //#00FF00
const color verde_oscuro = hexToImVec4("008000"); //#008000

