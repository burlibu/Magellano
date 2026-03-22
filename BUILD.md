# Magellano: Cross-Platform Build Guide

Questo documento descrive come compilare Magellano su Linux e Windows.

## Requisiti Comuni

- **CMake**: >= 3.16
- **C++ Compiler**: C++17 support (GCC 7+, Clang 5+, MSVC 2017+)
- **OpenGL**: >= 3.0
- **Dependencies**:
  - GLFW3
  - OpenGL
  - libcurl
  - (ImGui e JSON sono inclusi nel repository)

---

## Linux (Ubuntu/Debian)

### 1. Installa dipendenze

```bash
sudo apt-get update
sudo apt-get install -y \
  cmake \
  build-essential \
  libgl1-mesa-dev \
  libglfw3-dev \
  libcurl4-openssl-dev \
  pkg-config
```

### 2. Clona e compila

```bash
cd ~/Documents/Projects/Magellano
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 3. Esegui

```bash
./magellano
```

---

## Windows (Visual Studio 2017+)

### Opzione A: CMake + vcpkg (CONSIGLIATO)

#### Prerequisiti

1. **Visual Studio**: Con "Desktop development with C++" workload
   - Download: https://visualstudio.microsoft.com/

2. **CMake**: >= 3.16
   - Download: https://cmake.org/download/

3. **vcpkg** (Package Manager per Windows)
   ```bash
   cd C:\
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   ```

#### Build Steps

```bash
# 1. Clona il repository
cd C:\Projects
git clone https://github.com/burlibu/Magellano.git
cd Magellano

# 2. Installa dipendenze via vcpkg
C:\vcpkg\vcpkg install glfw3:x64-windows curl:x64-windows opengl:x64-windows

# 3. Configura con CMake
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake

# 4. Compila
cmake --build . --config Release -j%NUMBER_OF_PROCESSORS%

# 5. Esegui
.\Release\magellano.exe
```

### Opzione B: Installazione manuale di librerie

1. **Scarica e installa**:
   - GLFW: https://www.glfw.org/download
   - OpenGL: Di solito fornito dal driver GPU (NVIDIA/AMD/Intel)
   - libcurl: https://curl.se/windows eller pre-compiled: https://www.libcurl.org

2. **Configura CMake**:
   ```bash
   cmake .. -G "Visual Studio 17 2022" -A x64 `
     -DGLFW3_DIR="C:\path\to\glfw" `
     -DCURL_DIR="C:\path\to\curl"
   ```

---

## macOS

### 1. Installa Homebrew (se non presente)

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 2. Installa dipendenze

```bash
brew install cmake glfw opengl curl pkg-config
```

### 3. Compila

```bash
cd ~/Projects/Magellano
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
./magellano
```

---

## Troubleshooting

### Errore: "curl not found"

**Linux:**

```bash
sudo apt-get install libcurl4-openssl-dev
```

**Windows (vcpkg):**

```bash
C:\vcpkg\vcpkg install curl:x64-windows
```

**macOS:**

```bash
brew install curl
```

### Errore: "CMake not found"

Scarica da https://cmake.org/download e segui le istruzioni di installazione.

### Architetture diverse su Windows

Se il tuo Windows è 32-bit (raro), usa `x86-windows` al posto di `x64-windows` nel comando vcpkg.

---

## Configurazione IDE

### Visual Studio Code

1. **Estensioni raccomandate**:
   - C/C++ (Microsoft)
   - CMake Tools (Microsoft)
   - Makefile Tools (Microsoft)

2. **Copia `.vscode` dal repository** o crea manualmente in `.vscode/settings.json`:
   ```json
   {
     "cmake.configureSettings": {
       "CMAKE_TOOLCHAIN_FILE": "C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake"
     }
   }
   ```

### Visual Studio

1. Apri CMake → "Open Folder" → Seleziona cartella Magellano
2. CMake automaticamente configura e mostra Build menu:
   - Build → Build All (Ctrl+Shift+B)

---

## CI/CD (GitHub Actions)

Il repository include `.github/workflows/build.yml` che compila automaticamente su:

- Ubuntu 20.04 (Linux)
- Windows Server 2022
- macOS Latest

Output binaries salvati come artifacts per download.

---

## C++ Standard

**Magellano richiede C++17** (per `std::filesystem`).

Se hai un compilatore ameno, assicurati di abilitare C++17:

**GCC/Clang:**

```bash
cmake .. -DCMAKE_CXX_STANDARD=17
```

**MSVC:**
Automatico in `CMakeLists.txt` (/std:c++17)

---

## Performance + Ottimizzazioni

Per build Release (ottimizzato):

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

Per Debug (con simboli di debug):

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

---

## Problemi comuni

| Problema                | Soluzione                                    |
| ----------------------- | -------------------------------------------- |
| `GLFW not found`        | Installa GLFW via package manager o vcpkg    |
| `CURL not found`        | Installa libcurl via package manager o vcpkg |
| `Compiler too old`      | Aggiorna a GCC 7+, Clang 5+, o MSVC 2017+    |
| `CMake version too old` | Aggiorna CMake a >= 3.16                     |

---

## Link Utili

- [CMake Documentation](https://cmake.org/documentation/)
- [vcpkg Quick Start](https://github.com/Microsoft/vcpkg#quick-start-windows)
- [GLFW Installation Guide](https://www.glfw.org/docs/latest/build_guide.html)
- [libcurl Installation](https://curl.se/download.html)
