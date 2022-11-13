#define SDL_MAIN_HANDLED
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define STB_IMAGE_IMPLEMENTATION

#include "../../vendor/SDL/include/SDL.h"
#include "../../vendor/SDL/include/SDL_stdinc.h"

#include "../../vendor/glm/glm/glm.hpp"

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

#include "../../vendor/portaudio/include/portaudio.h"

#include "../vendor/stb/stb_image.h"
#include "../vendor/stb/stb_image_write.h"

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

#include "./include/audio.hpp"
#include "./include/renderer.hpp"
#include "./include/windowUi.hpp"
#include "./include/input.hpp"
#include "./include/game.hpp"

static SDL_Window *window;
static SDL_GLContext glContext;

static inline void program_exit(bool val);
static inline void mainLoop();

#include "windowUi.cpp"
#include "audio.cpp"
#include "renderer.cpp"
#include "input.cpp"
#include "game.cpp"

