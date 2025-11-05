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
  
  
  
  Error FrameWindowManager::RenderFrame(const ImVec4& fwbc) {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(frame_window, &display_w, &display_h);
    
    // Aggiorna automaticamente le dimensioni della classe e le variabili globali
    frame_window_width = display_w;
    frame_window_heigth = display_h;
    frame_window_width_setting = display_w;
    frame_window_height_setting = display_h;
    
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
}