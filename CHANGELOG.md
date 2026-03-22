# CHANGELOG

## Version 0.0.1

### 12/7/2025

// TODO creare un sistema di salvataggio configurazioni e dati magari con json. C' è il video skyes molto utile
// TODO spostare le variabili del progetto dal main a un file apposito

---

## Version 0.0.2

### 13/7/2025

- Implementate funzioni di supporto, creati file di environment e settings, corretti bug minori.
- Dato una struttura più robusta al progetto
- Avviata documentazione funzioni con Doxygen e creato Doxygen docs in docs/

---

## Version 0.0.3

### 16/7/2025

- Ristrutturato tutto il codice attraverso classi
- TODO: Creare un metodo per calcolare quali pixel sono occupati da delle finestre in runtime in modo da
  riuscire a creare finestre dinamicamente
- TODO: All' avvio mostrare solo il login e mostrare le altre finestre dopo il login.

---

## Version 0.0.4

## 5/8/2025

- Sistemato l' intero schermo coprendolo con delle finestre

## Version 0.0.5

## 3/9/2025

- Definito lo scope dell' applicazione: hacking and cybersecurity
- Aggiunta la gestione degli attacchi nel GUI
- Aggiunta la visualizzazione della cronologia degli attacchi tramite un sistema di scrittura e lettura di un file json
- Aggiunta dipendenza nlohmann/json per la gestione dei file json
- Aggiunti errori per ip e port
- Fixed Help tab
- Cambiato nome del progetto in Magellano

## Version 0.0.6

## 21/03/2026

- Aggiunta la gestione della rete con la visualizzazione delle interfacce di rete e delle connessioni attive
- BUG: attack log box: non si inqueadra chiaramente e le scritte escono fuori
- Ora le notifiche vengono renderizzate correttamente nel loop principale
- Aggiunta la gestione della persistenza dei dati con file json
- Molte impostazioni sono ancora placeholder e non funzionano
- Risoluzione di default: 1920x1080 per evitare problemi di posizionamento finestre
- centralizzazione gestione finestre autenticate, resa robusta la persistenza JSON, notifiche correttamente renderizzate nel loop principale.
- Monitoraggio realt-time risolzione
- Tab Changelog

## Version 0.1.0 - Stabile

## 22/03/2026

- Cambiata gestione bool windows in data registry per miglior efficienza e manutentabilità

---

## Version 0.1.1 - Cross-Platform (Currently in progress)

## 22/03/2026

### Major Changes

- **Cross-Platform Support**: Magellano ora compila e funziona sia su Linux che su Windows
- **Build System**: Migrato da Makefile a CMake 3.16+ per gestione cross-platform automatica
- **C++ Standard**: Aggiornato a C++17 (std::filesystem) per filesystem handling platform-agnostic
- **Registry Pattern**: Implementato per gestione centralizzata delle window toggles (vedi Area D del branchfeature/windows-support)

### Technical Improvements

- **File I/O**: Sostituito `sys/stat.h` + `mkdir()` con `std::filesystem::create_directories()`
  - Codice unico tra Linux e Windows
  - Gestione automatica di path separators (`/` vs `\`)
  - Compatibile con tutti i filesystem moderni
- **Network/Process Handling**: Aggiunto supporto per command detection cross-platform
  - Linux: usa `command -v` per verificare presenza di comandi
  - Windows: usa `where` per verificare presence di comandi
  - Codice singolo con condizionali `#ifdef _WIN32`
- **Changelog Fetcher**: Migliorato con supporto platform-specific per libcurl
  - Messaggi di errore dettagliati per installazione curl su Windows/Linux/macOS
  - Fallback automatico entre main/master branch

### Build Documentation

- Aggiunto `BUILD.md` con istruzioni per compilare su:
  - Linux (Ubuntu/Debian preferred)
  - Windows (Visual Studio + vcpkg recommended)
  - macOS (Homebrew)

### Platform-Specific Notes

#### Windows

- Dipendenza: libcurl (installare via vcpkg, Chocolatey, o manualmente)
- Build: `cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake`
- Esecuzione: `.\Release\magellano.exe`

#### Linux

- Dipendenze: libcurl4-openssl-dev, libglfw3-dev, libgl1-mesa-dev
- Build: `mkdir build && cd build && cmake .. && make -j$(nproc)`
- Esecuzione: `./magellano`

### Backward Compatibility

- ✅ Nessun breaking change per codice Linux esistente
- ✅ Stessa funcionalità su tutte le piattaforme supportate
- ✅ Build system automatico rileva piattaforma e configura di conseguenza
- ⚠️ Richiede C++17+ compiler (GCC 7+, Clang 5+, MSVC 2017+)

### Known Issues

- Primera volta che si esegue su Windows richiede cicca 'Refresh' nel Changelog tab per download CHANGELOG.md
- Path Unicode non ancora testati (legacy ANSI supportato)

### non testato su windows, potrebbe avere problemi di path o dipendenze. Testare e correggere se necessario.

# Version 0.1.2

- Aggiornato network monitor per mostrare statistiche interfacce di rete in tempo reale
- Aggiunta gestione errori per interfacce di rete non accessibili o senza permessi
- Ottimizzato rendering del network monitor per ridurre lag su sistemi con molte

# Version 0.1.3

- Aggiornata Tab window
