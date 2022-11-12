#include "main.hpp"


int main()
{
    initUi();
    initRender();

    mainLoop();

    program_exit(0);
    return 0;
}

static inline void mainLoop()
{
    while(true)
    {
        updateInput();
        render();
    }
}

static inline void program_exit(bool val)
{
    printf("closing");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(val);
}