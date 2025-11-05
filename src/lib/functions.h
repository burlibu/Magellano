#pragma once
//standards
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include <random>
#include <regex>
#include <ctime>
#include <algorithm>
// imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
//src

#include "../settings/env.h"
#include "../settings/settings.h"
#include "../lib/custom_colors.h"
//json
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

/**
 * @brief Funzione di debug che stampa l' argomento dato solo se il bool DEBUG da env.h è true
 * 
 * @tparam T 
 * @param a 
 */
template<typename T>
void db(const T& a) {
  if (DEBUG) cout << a << "\n";
}
/**
 * @brief Funzione di debug che stampa gli argomenti dati solo se il bool DEBUG da env.h è true
 * 
 * @tparam T 
 * @tparam Args 
 * @param first 
 * @param rest 
 */
template<typename T, typename... Args>
void db(const T& first, const Args&... rest) {
    if (DEBUG) std::cout << first << " ";
    db(rest...);
}

enum class Error {
    OK,
    FAIL
};

enum class NotificationType {
    INFO,    ///< Notifica informativa
    WARNING, ///< Notifica di avvertimento
    ERROR,   ///< Notifica di errore
    SUCCESS  ///< Notifica di successo
};
struct Notification;

/**
 * @brief Struttura per contenere tutte le informazioni di sistema
 */
struct SystemInfo {
    std::string deviceName;
    std::string osName;
    std::string osVersion;
    std::string cpuInfo;
    size_t totalRamMB;
    size_t freeRamMB;
};

/**
 * @brief Funzione che attiva un bool di una finestra solo se nelle impostazioni è attivo
 * 
 * @param b 
 * @param v 
 */
void setBool(bool &b, bool v);

namespace Math {
    /**
 * @brief Funzione che calcola il fattoriale 
 * @param[in] n numero intero per calcolo fattoriale >= 0
 * @return  `res` numero intero risultato del calcolo
 */
    unsigned int fact(unsigned int n);
}


/**
 * @brief Function to create a personalized style
 * 
 */
void style();
/*! 
@brief Mostra una notifica temporanea nell'interfaccia.
 * 
 *  Questa funzione crea una notifica che apparirà nell'interfaccia per un tempo
 *  specificato. La notifica verrà automaticamente rimossa dopo la scadenza del
 *  tempo impostato.
 *
 *  Le notifiche vengono visualizzate nell'angolo in basso a destra dello schermo
 *  e possono essere utilizzate per informare l'utente su eventi, errori o
 *  conferme di azioni.
 *
 *  @param[in] title const std::string& Il titolo della notifica, visualizzato in grassetto.
 *  @param[in] text const std::string& Il testo del messaggio della notifica.
 *  @param[in] duration_ms int La durata di visualizzazione in millisecondi.
 *
 *  @errors Nessun errore specifico, ma se la memoria è insufficiente la notifica
 *  potrebbe non essere creata.
 *
 *  @thread_safety Questa funzione può essere chiamata da qualsiasi thread.
 *
 *  @sa RenderNotifications
 *  @sa Notification
 *
 *  @since Versione 1.0
 *
 *  @ingroup notifications
 */
void showNotification(const std::string& title, const std::string& text, int duration_ms); //MEMO6
/**
 * @brief Funzione che prende una stringa colore come bde0fe o bde0fe80 e ritorna un ImVec4 corrispettivo 
 * @param[in] n numero intero per calcolo fattoriale >= 0
 * @return  `res` numero intero risultato del calcolo
 * @note Supporta stringhe da 6 o 8 caratteri
 */
ImVec4 hexToImVec4(const std::string& hex);
/**
 * @brief Funzione che disegna un rettangolo che si illumina
 */
void DrawGlowingRectangle();
/*! @brief Esegue un carico di lavoro intensivo simulato.
 *
 *  @details Questa funzione simula un'operazione computazionalmente pesante
 *  che richiede un tempo significativo per essere completata. È progettata
 *  per testare la gestione di operazioni asincrone e la reattività dell'interfaccia
 *  utente durante l'esecuzione di task lunghi.
 *
 *  @note La funzione attualmente simula un lavoro di 5 secondi tramite
 *  \c std::this_thread::sleep_for. In un'implementazione reale, questo
 *  dovrebbe essere sostituito con calcoli effettivi.
 *
 *  @warning Questa funzione blocca il thread corrente per 5 secondi.
 *  Non chiamare questa funzione dal thread principale dell'interfaccia
 *  grafica senza utilizzare thread separati.
 *
 *  @attention Per un uso corretto, considera l'implementazione di thread
 *  separati per evitare il blocco dell'interfaccia utente.
 *
 *  @remark La funzione è progettata per essere utilizzata come esempio
 *  di gestione di operazioni asincrone in applicazioni ImGui.
 *
 *  @pre Il sistema deve essere inizializzato correttamente.
 *  @post Nessun effetto collaterale permanente.
 *
 *  @invariant La funzione mantiene la coerenza del sistema.
 *
 *  @complexity O(1) - tempo costante di 5 secondi.
 *
 *  @deprecated Questa funzione è deprecata. Usa \c lavoro_lungo() invece.
 *
 *  @see lavoro_lungo
 *  @see esempio_future
 *  @see std::thread
 *  @see std::chrono::sleep_for
 *
 *  @par Esempio di utilizzo:
 *  @code
 *  // Creazione di un thread separato per evitare il blocco dell'UI
 *  std::thread worker(hardLoad);
 *  worker.detach();
 *  @endcode
 *
 *  @par Implementazione attuale:
 *  @code
 *  void hardLoad() {
 *      std::this_thread::sleep_for(std::chrono::seconds(5));
 *  }
 *  @endcode
 *
 *  @bug Nessun bug noto.
 *  @todo Implementare un sistema di progress tracking.
 *  @todo Aggiungere supporto per cancellazione dell'operazione.
 *
 *  @exception Non lancia eccezioni.
 *  @throws Nessuna eccezione.
 *
 *  @return void
 *  @retval Nessun valore di ritorno.
 *
 *  @since Versione 1.0
 *  @version 1.0
 *  @author Alberus
 *  @date 2024
 *  @copyright Tutti i diritti riservati.
 *
 *  @ingroup thread_operations
 *  @defgroup thread_operations Operazioni con Thread
 *  @{
 *  Funzioni per la gestione di operazioni asincrone e thread.
 *  @}
 *
 *  @anchor hardLoad_example
 *  @ref hardLoad_example "Vedi esempio completo"
 *
 *  @relatesalso lavoro_lungo
 *  @relatesalso esempio_future
 *
 *  @hideinitializer
 *  @hidecallgraph
 *  @hidecallergraph
 *
 *  @implements ThreadOperation
 *  @extends BaseOperation
 *  @implements AsyncOperation
 *
 *  @par Performance:
 *  - Tempo di esecuzione: 5 secondi
 *  - Utilizzo memoria: Costante
 *  - Utilizzo CPU: Minimo (sleep)
 *
 *  @par Sicurezza:
 *  - Thread-safe: Sì
 *  - Reentrant: Sì
 *  - Exception-safe: Sì
 *
 *  @par Compatibilità:
 *  - C++11 o superiore
 *  - Windows/Linux/macOS
 *  - Compilatori: GCC, Clang, MSVC
 *
 *  @par Note di implementazione:
 *  La funzione utilizza \c std::this_thread::sleep_for per simulare
 *  un lavoro intensivo. In un'implementazione reale, questo dovrebbe
 *  essere sostituito con calcoli effettivi o operazioni I/O.
 *
 *  @par Best practices:
 *  - Utilizzare sempre thread separati
 *  - Implementare un sistema di feedback per l'utente
 *  - Considerare l'utilizzo di \c std::future per risultati
 *
 *  @par Esempi avanzati:
 *  @code
 *  // Con progress tracking
 *  std::atomic<int> progress{0};
 *  std::thread worker([&progress]() {
 *      for(int i = 0; i < 100; ++i) {
 *          std::this_thread::sleep_for(std::chrono::milliseconds(50));
 *          progress = i;
 *      }
 *  });
 *  @endcode
 */
void HardLoad();
/**
 * 
 */
int lavoro_lungo();
/**
 * 
 */
void esempio_future();
/**
 * Prende un ImVec2 e lo trasforma in una stringa per esempio (200, 300) diventa "200, 300" 
 * @param vector un imvec2
 */
std::string ImVec2_to_string(const ImVec2& vector );
/**
 * Controlla ogni 5 secondi se la size della finestra è cambiata e lo scrive in terminal
 * @param currentsize size attuale quando viene chiamata la funzione
 * @param checkIntervalSeconds time between size checks
 * @return bool
 */
bool checkWindowSizeChange(const ImVec2& currentSize, float checkIntervalSeconds = 5.0f);
/**
 * Prende il puntatore ad una finestra e ne restituisce le coordinate in mezzo 
 */
ImVec2 CenterPosWindow(ImVec2 imvec2, GLFWwindow* window_ptr);
/**
 * Creates a Rectangle (formally a window) from a position and a size
 * @param pos position of the rectangle
 * @param size size of the rectangle
 * @param col color of the rectangle
 * @return Error
 */
Error Rectangle(const ImVec2& pos, const ImVec2& size, color col);

/**
 * Esegue un attacco simulato verso un target.
 * @param target_ip IP del bersaglio
 * @param port Porta di destinazione
 * @param attack_type Tipo di attacco (es: "DoS", "BruteForce", ecc.)
 * @param claymore Abilitazione del metodo Claymore
 * @param spread Abilitazione del metodo Spread
 * @param network_spread Abilitazione del metodo Network Spread
 * @param err Array con size = 8 di codici da utilizzare per gli errori
 * @param [0] Errore IP
 * @param [1] Errore Port
 * @param [2] not set
 * @return true se l'attacco è stato eseguito, false altrimenti
 */
void attack(const std::string& target_ip, const int& port, const std::string& attack_type,
            bool claymore, bool spread, bool network_spread, int err[]);

void save_attack_json(const std::string& ip, const int& port, const std::string& attack_type, bool claymore, bool spread, bool network_spread, bool success, std::string timestamp);

/**
 * @brief Controlla se un indirizzo IP è valido.
 *
 * @param ip L'indirizzo IP da controllare.
 * @return Error::OK se l'IP è valido, Error::FAIL altrimenti.
 */
Error checkIp(const std::string& ip);

/**
 * @brief Transforma un time_t in una stringa leggibile (formato: YYYY-MM-DD HH:MM:SS)
 * 
 */
std::string time_t_to_string(time_t t);

// Funzioni di sistema
/**
 * @brief Get the device name object
 * 
 * @return std::string 
 */
std::string get_device_name();

/**
 * @brief Get the os info object
 * 
 * @return std::string 
 */
std::string get_os_info();

/**
 * @brief Get the cpu info object
 * 
 * @return std::string 
 */
std::string get_cpu_info();

/**
 * @brief Get the total ram mb object
 * 
 * @return size_t 
 */
size_t get_total_ram_mb();

/**
 * @brief Get the free ram mb object
 * 
 * @return size_t 
 */
size_t get_free_ram_mb();

/**
 * @brief Get the system information
 * 
 * @return SystemInfo 
 */
SystemInfo get_system_info();

/**
 * @brief Get the primary monitor resolution
 * 
 * @param width pointer to store monitor width
 * @param height pointer to store monitor height
 * @return true if successful, false otherwise
 */
bool get_monitor_resolution(int* width, int* height);

/**
 * @brief Calculate optimal window size based on monitor resolution
 * 
 * @param scale_factor percentage of monitor size to use (0.0 to 1.0)
 * @return std::pair<int, int> width and height
 */
std::pair<int, int> calculate_window_size(float scale_factor = 0.8f);

/**
 * @brief Initialize window settings with dynamic resolution
 * 
 * @param scale_factor percentage of monitor size to use (0.0 to 1.0)
 */
void initialize_window_settings(float scale_factor = 0.8f);

/**
 * @brief Update ImGui windows to fit current screen size
 * 
 * @param window_width current window width
 * @param window_height current window height
 */
void update_imgui_layout(int window_width, int window_height);


