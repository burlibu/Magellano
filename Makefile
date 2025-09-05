# Makefile per progetto ImGui
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
INCLUDES = -Iinclude -Iexternal/imgui -Iexternal/imgui/backends -Iassets -Iexternal/json

# File sorgente del tuo progetto
PROJECT_SOURCES = src/main.cpp src/custom_colors.cpp src/env.cpp src/settings.cpp src/functions.cpp \
     src/gui/Attack.cpp src/gui/BottomBar.cpp src/gui/Demo.cpp src/gui/FrameManager.cpp \
    src/gui/Help.cpp src/gui/Login.cpp src/gui/MenuBar.cpp src/gui/MinimizeAndExit.cpp src/gui/Settings.cpp \
    src/gui/ShowMyWindow.cpp src/gui/Table.cpp src/gui/TabWindow.cpp src/gui/Tree.cpp src/gui/Window1.cpp

# File sorgente di Dear ImGui
IMGUI_SOURCES = external/imgui/imgui.cpp \
                external/imgui/imgui_demo.cpp \
                external/imgui/imgui_draw.cpp \
                external/imgui/imgui_tables.cpp \
                external/imgui/imgui_widgets.cpp

# Backend (scegli quello che preferisci)
BACKEND_SOURCES = external/imgui/backends/imgui_impl_glfw.cpp \
                  external/imgui/backends/imgui_impl_opengl3.cpp

# Tutti i file sorgente
SOURCES = $(PROJECT_SOURCES) $(IMGUI_SOURCES) $(BACKEND_SOURCES)

# Nome dell'eseguibile
EXE = magellano

# Librerie necessarie
LIBS = -lGL $(shell pkg-config --static --libs glfw3)

# Regole di compilazione
all: $(EXE)

$(EXE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

clean:
	rm -f $(EXE)

.PHONY: all clean