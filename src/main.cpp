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
#include "gui/gui.h"


int main() {
// Inizializza GLFW
if (!glfwInit()) {
std::cerr << "Errore nell'inizializzazione di GLFW" << std::endl;
return -1;
}

// Crea la finestra
Gui::FrameWindowManager frame_window_manager(frame_window_width_setting,frame_window_heigth_setting,frame_window_title_setting,frame_window_monitor_setting,
                                        frame_window_share_setting);
frame_window_manager.Create();


glfwMakeContextCurrent(frame_window_manager.getWindow());
glfwSwapInterval(1); // VSync

// Inizializza Dear ImGui
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

// Setup style
ImGui::StyleColorsDark();

// Setup Platform/Renderer backends
ImGui_ImplGlfw_InitForOpenGL(frame_window_manager.getWindow(), true);
ImGui_ImplOpenGL3_Init("#version 130");

//calcolo width and height finestra in tempo reale
int frame_width, frame_height;
float bottombar_height = 35.0f;

glfwGetFramebufferSize(frame_window_manager.getWindow(), &frame_width, &frame_height);
int menubar_height = static_cast<int>(ImGui::GetFrameHeight());
GLFWwindow* frame_p = frame_window_manager.getWindow(); // pointer to the frame

// Istanziazione classi finestre
Gui::MinimizeAndExitWindow minimize_and_exit_window(ImVec2(0,0),ImVec2(frame_width,35.0f), frame_p,flags_minimize_and_exit_window);
Gui::LoginWindow loginWin(ImVec2(100, 100), ImVec2(500, 400), frame_p, flags_login_window);
Gui::ShowMyWindow mywin(ImVec2(100, 100), ImVec2(500, 400), frame_p, flags_my_window); // classe loginWindow creata
Gui::showDemoWindow demoWindow(ImVec2(400,400), ImVec2(200,200), frame_p,flags_demo_window);

Gui::Window1 window1(ImVec2(100, 100), ImVec2(500, 400), frame_p, flags_window1);
Gui::MenuBar menubar(ImVec2(0,0), ImVec2(0,0), frame_p, flags_menu_bar);

Gui::TabWindow tabWindow("TabWindow", ImVec2(0,18), ImVec2(frame_width/6,frame_window_manager.frame_window_heigth - bottombar_height - menubar_height), frame_p, flags_tab_window);
Gui::Tree tree("Tree", ImVec2(frame_width/6,menubar_height), ImVec2(2*frame_width/6,400), frame_p, flags_tree);
Gui::Table table("Attack Log", ImVec2(3*frame_width/6,menubar_height), ImVec2(3*frame_width/6,400), frame_p, flags_table);
Gui::AttackWindow attack_window(ImVec2(frame_width/6,400+18), ImVec2((5*frame_width/6),(frame_window_manager.frame_window_heigth -400 - menubar_height - bottombar_height)), frame_p,flags_attack_window);


Gui::BottomBar bottomBar("BottomBar", ImVec2(500,500), ImVec2(frame_width, 35.0f), frame_p, flags_BottomBar);
Gui::HelpWindow helpWindow("Help", ImVec2(0,0 + menubar.GetPos().y), ImVec2(frame_window_manager.frame_window_width,frame_window_manager.frame_window_heigth - menubar.GetPos().y), frame_p, flags_HelpWindow);
Gui::SettingsWindow settingsWindow(ImVec2(0,0 + menubar.GetPos().y), ImVec2(frame_window_manager.frame_window_width,frame_window_manager.frame_window_heigth - menubar.GetPos().y), frame_p, flags_settings);
Gui::NetworkWindow networkWindow("Network Monitor", ImVec2(frame_width / 6, menubar_height), ImVec2(5 * frame_width / 6, frame_height - menubar_height - bottombar_height), frame_p, flags_network);
Gui::ChangelogWindow changelogWindow("Changelog", ImVec2(0, static_cast<float>(menubar_height)), ImVec2(static_cast<float>(frame_width), frame_height - menubar_height - bottombar_height), frame_p, flags_changelog);










//* /////////////////////////////////////////////////////////// Loop principale ///////////////////////////////////////////////////////
while (!glfwWindowShouldClose(frame_window_manager.getWindow())) {
glfwPollEvents();

// Start the Dear ImGui frame
ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplGlfw_NewFrame();
ImGui::NewFrame();

// Layout dinamico: ricalcola sempre in base alla risoluzione corrente.
glfwGetFramebufferSize(frame_window_manager.getWindow(), &frame_width, &frame_height);
menubar_height = static_cast<int>(ImGui::GetFrameHeight());

tabWindow.SetPos(ImVec2(0, static_cast<float>(menubar_height)));
tabWindow.SetSize(ImVec2(frame_width / 6.0f, frame_height - bottombar_height - menubar_height));

tree.SetPos(ImVec2(frame_width / 6.0f, static_cast<float>(menubar_height)));
tree.SetSize(ImVec2((2.0f * frame_width) / 6.0f, 400.0f));

table.SetPos(ImVec2((3.0f * frame_width) / 6.0f, static_cast<float>(menubar_height)));
table.SetSize(ImVec2((3.0f * frame_width) / 6.0f, 400.0f));

attack_window.SetPos(ImVec2(frame_width / 6.0f, 400.0f + menubar_height));
attack_window.SetSize(ImVec2((5.0f * frame_width) / 6.0f, frame_height - 400.0f - menubar_height - bottombar_height));

networkWindow.SetPos(ImVec2(frame_width / 6.0f, static_cast<float>(menubar_height)));
networkWindow.SetSize(ImVec2((5.0f * frame_width) / 6.0f, frame_height - menubar_height - bottombar_height));

changelogWindow.SetPos(ImVec2(0.0f, static_cast<float>(menubar_height)));
changelogWindow.SetSize(ImVec2(static_cast<float>(frame_width), frame_height - menubar_height - bottombar_height));

bottomBar.SetSize(ImVec2(static_cast<float>(frame_width), bottombar_height));
//^ ///////////////////////////////////////////////////////// Login Window ///////////////////////////////////////////////////////////
if (bool_login_window) {
    loginWin.Render();
    loginWin.isLogged(); // TODO da rendere più sicuro ed efficiente
    setAuthenticatedWindows(loginWin.logged);
}


//£ ///////////////////////////////////////////////////////My Window /////////////////////////////////////////////////////////////////
if (bool_my_window) {
    mywin.Render();
}


//! ////////////////////////////////////////////////MINIMIZE AND EXIT WINDOW /////////////////////////////////////////////////////////
if (bool_minimize_and_exit_window) {
    minimize_and_exit_window.Render();
}


//*  ////////////////////////////////////////////////////////// Window1 ///////////////////////////////////////////////////////////////
if (bool_window1) {
    window1.Render();
}

// TODO /////////////////////////////////////////////////////// Demo Window ////////////////////////////////////////////////////////////
if (bool_demo_window) {
    demoWindow.Render();
}

//! ///////////////////////////////////////////////////////// Attack window //////////////////////////////////////////////////////
if (bool_attack_window) {
    attack_window.Render();
}


///////////////////////////////////////////////////////////// Menu Bar ///////////////////////////////////////////////////////////////
if (bool_menu_bar) {
    menubar.Render();
}

//////////////////////////////////////////////////////////// Tab Window /////////////////////////////////////////////////////////////////
if (bool_tab_window) {
    tabWindow.Render();
}

/////////////////////////////////////////////////////////// Tree ////////////////////////////////////////////////////////////////////////
if (bool_Tree) {
tree.Render();
}
/////////////////////////////////////////////////////////// Table //////////////////////////////////////////////////////////////////////////
if (bool_table) {
table.Render();
}
/////////////////////////////////////////////////////////// Settings Window //////////////////////////////////////////////////////////////////////////
if (bool_settings) {
settingsWindow.Render();
}
if (bool_HelpWindow)
{
helpWindow.Render();
}
if (bool_network)
{
networkWindow.Render();
}
if (bool_changelog)
{
changelogWindow.Render();
}
if (bool_BottomBar)
{
bottomBar.Render();
}

RenderNotifications();
element_id = 0;

// Rectangle(ImVec2(100,100), ImVec2(200,200), viola);
// Rectangle(ImVec2(600,100), ImVec2(100,200), rosso);
// Rectangle(ImVec2(300,500), ImVec2(100,50), blu_chiaro);
// Rectangle(ImVec2(400,500), ImVec2(100,50), verde);
// Rectangle(ImVec2(500,500), ImVec2(100,50), rosa);





// Rendering frame principale
frame_window_manager.RenderFrame(blu);
} // fine while principale

// Cleanup
frame_window_manager.CleanUp();

return 0;
}