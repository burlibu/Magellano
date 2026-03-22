cmake_minimum_required(VERSION 3.16)
project(Magellano)

# Imposta C++17 come standard (necessario per std::filesystem)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Supporto automatico per vcpkg (Windows consigliato)
# Se usi vcpkg, passa: -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND WIN32)
    message(WARNING "CMAKE_TOOLCHAIN_FILE not set. On Windows with vcpkg, use:")
    message(WARNING "  cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
endif()

# Configurazione per Visual Studio (Windows)
if(MSVC)
    # Abilita conformità C++17 stretta
    add_compile_options(/std:c++17 /W4)
else()
    # GCC/Clang (Linux, macOS)
    add_compile_options(-Wall -Wextra -g)
endif()

# =============================================================================
# RICERCA DIPENDENZE
# =============================================================================

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(CURL REQUIRED)

# =============================================================================
# SORGENTI PROGETTO
# =============================================================================

set(PROJECT_SOURCES
    src/main.cpp
    src/custom_colors.cpp
    src/env.cpp
    src/settings.cpp
    src/functions.cpp
    src/gui/Attack.cpp
    src/gui/BottomBar.cpp
    src/gui/Demo.cpp
    src/gui/FrameManager.cpp
    src/gui/Help.cpp
    src/gui/Login.cpp
    src/gui/MenuBar.cpp
    src/gui/MinimizeAndExit.cpp
    src/gui/Settings.cpp
    src/gui/ShowMyWindow.cpp
    src/gui/Table.cpp
    src/gui/TabWindow.cpp
    src/gui/Tree.cpp
    src/gui/Window1.cpp
    src/gui/Network.cpp
    src/gui/Changelog.cpp
)

# Sorgenti ImGui
set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
)

# Backend ImGui (GLFW + OpenGL)
set(BACKEND_SOURCES
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

# =============================================================================
# ESECUTABILETubo
# =============================================================================

add_executable(magellano
    ${PROJECT_SOURCES}
    ${IMGUI_SOURCES}
    ${BACKEND_SOURCES}
)

# =============================================================================
# INCLUDE DIRECTORIES
# =============================================================================

target_include_directories(magellano PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_SOURCE_DIR}/external/json
)

# =============================================================================
# LINK LIBRARIES
# =============================================================================

target_link_libraries(magellano PRIVATE
    OpenGL::OpenGL
    glfw
    CURL::libcurl
)

# Platform-specific linking
if(WIN32)
    # Windows-specific libraries (if needed in future)
    # target_link_libraries(magellano PRIVATE Winhttp)
elseif(UNIX AND NOT APPLE)
    # Linux-specific libraries
    target_link_libraries(magellano PRIVATE m)  # Math library
endif()

# =============================================================================
# POST BUILD
# =============================================================================

# Copia l'eseguibile nella root per consistent behavior
if(WIN32)
    set_target_properties(magellano PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
else()
    set_target_properties(magellano PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()

# =============================================================================
# PRINT CONFIG
# =============================================================================

message(STATUS "===============================================")
message(STATUS "Magellano Cross-Platform Build Configuration")
message(STATUS "===============================================")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "OpenGL: ${OPENGL_FOUND}")
message(STATUS "GLFW3: ${glfw3_FOUND}")
message(STATUS "CURL: ${CURL_FOUND}")
message(STATUS "===============================================")
