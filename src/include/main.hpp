#define SDL_MAIN_HANDLED
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define STB_IMAGE_IMPLEMENTATION
//#define BOOST_NO_EXCEPTIONS

#include "../../vendor/SDL/include/SDL.h"
#include "../../vendor/SDL/include/SDL_stdinc.h"

#include "../../vendor/glm/glm/ext.hpp"

#include "../../vendor/glad/include/glad/glad.h"
#include "../../vendor/glad/src/glad.c"

#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/backends/imgui_impl_opengl3.h"
#include "../../vendor/imgui/backends/imgui_impl_sdl.h"

#include "../../vendor/imgui/imgui.cpp"
#include "../../vendor/imgui/backends/imgui_impl_sdl.cpp"
#include "../../vendor/imgui/imgui_draw.cpp"
#include "../../vendor/imgui/imgui_tables.cpp"
#include "../../vendor/imgui/imgui_widgets.cpp"
#include "../../vendor/imgui/backends/imgui_impl_opengl3.cpp"


#ifndef __GNUC__
#undef vsnprintf _vsnprintf
#endif
#include "../../vendor/boost/libs/dll/include/boost/dll/runtime_symbol_info.hpp"

#include "../../vendor/portaudio/include/portaudio.h"

#include "../vendor/stb/stb_image.h"
#include "../vendor/stb/stb_image_write.h"

#include <../vendor/assimp/include/assimp/Importer.hpp>
#include <../vendor/assimp/include/assimp/scene.h>
#include <../vendor/assimp/include/assimp/postprocess.h>

#include <string>
#include <vector>
#include <tuple>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cmath>
#include <atomic>
#include <cstdio>
#include <thread>
#include <ostream>
#include <cstdlib>
//#include <mutex>
#include <unordered_map>
#include<sstream>
#include <time.h>
#include <algorithm>
#include <cstdlib>

#include "./include/audio.hpp" 
#include "./include/textures.hpp"
#include "./include/renderer.hpp"
#include "./include/windowUi.hpp"
#include "./include/input.hpp"
#include "./include/game.hpp"
#include "./include/rendererB.hpp"
#include "./include/modelImport.hpp"

static SDL_Window *window;
static SDL_GLContext glContext;

int windowwidth;
int windowheight;

//std::mutex mtx;

static inline void program_exit(bool val);
static inline void mainLoop();

Uint64 prevFrame = 0;
float deltaTime = 0;

#include "windowUi.cpp"
#include "gameEngine/audioEngine/audio.cpp"
#include "gameEngine/renderEngine/renderer.cpp"
#include "gameEngine/renderEngine/shaders.cpp"
#include "gameEngine/renderEngine/textures.cpp"
#include "gameEngine/renderEngine/modelImport.cpp"
#include "gameEngine/renderEngine/rendererB.cpp"
#include "gameEngine/inputEngine/input.cpp"
#include "game.cpp"
