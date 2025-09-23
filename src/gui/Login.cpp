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
#include "../env.h"
#include "../settings.h"
#include "../functions.h"
#include "../custom_colors.h"
#include "../crypto_utils.h"
#include "gui.h"

using namespace std;

namespace Gui {
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
              if (!encryption) {
                if (std::string(username) == _username_ && std::string(password) == _password_) {
                  login_success = true;
                } else {
                  login_success = false;
                }
              }
              else {
                  // In un sistema reale, recupereresti salt e hash dal database
                  // Per ora usiamo credenziali hard-coded con hash per demo
                if (std::string(username) == "admin") {
                    // Salt e hash pre-calcolati per "password"
                    std::string stored_salt = "3132333435363738393031323334353637383930"; // Salt fisso per demo
                    // Hash calcolato con: CryptoUtils::hash_password("password", stored_salt)
                    std::string stored_hash = "b109f3bbbc244eb82441917ed06d618b9008dd09b3befd1b5e07394c706a8bb980b1d7785e5976ec049b46df5f1326af5a2ea6d103fd07c95385ffab0cacbc86";
                    
                    // Verifica la password
                    login_success = CryptoUtils::verify_password(password, stored_hash, stored_salt);
                } else {
                    login_success = false;
                }
                
                if (login_success) {
                    // Esempio di come criptare dati dell'utente
                    std::string userInfo = "Username: " + std::string(username) + ", Ultimo accesso: " + time_t_to_string(time(nullptr));
                    std::string encryptedData = CryptoUtils::encrypt_string(userInfo, "magellano_secret_key");
                    
                    // In un sistema reale, salvare questi dati
                    // std::cout << "Dati criptati: " << encryptedData << std::endl;
                    
                    showNotification("Login", "Accesso effettuato con successo", 3000);
                } else {
                    showNotification("Login", "Credenziali non valide", 3000);
                }
                
                ImGui::Separator();
                
                // Informazioni di aiuto
                if (ImGui::IsItemHovered() && ImGui::BeginTooltip()) {
                  ImGui::Text("Credenziali di demo:");
                  ImGui::Text("Username: admin");
                  ImGui::Text("Password: password");
                  ImGui::EndTooltip();
                }
              }
            }
            
            if (login_success) {
                ImGui::TextColored(ImVec4(0,1,0,1), "Login Successful");
            } 
            else if (ImGui::Button("Hai dimenticato la password?")) {
                showNotification("Reset Password", "Funzionalità di reset password non ancora implementata", 3000);
            }
        }
        ImGui::Text("Versione: %s", version.c_str());
        ImGui::End();
    } // fine funzione Render

    void LoginWindow::isLogged(){
        if (login_success) {
            logged = true;
            db("Login effettuato con successo, stato logged = "); 
            db(logged);
        } else {
            logged = false;
        }
    }
}