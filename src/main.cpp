#include "main.hpp"


int main()
{
    initUi();
    initRender();
    initGame();

    mainLoop();

    //program_exit(0);
    return 0;
}

static inline void mainLoop()
{
    while(true)
    {
        gameLoop();
        updateInput();
        render();
    }
}

static inline void program_exit(bool val)
{
    printf("closing");

    for(textureObject* texture : textureObjects)
    {
        glDeleteTextures(1, &texture->textureID);
    }
    for(shaderObject* shader : shaderObjects)
    {
        glDeleteProgram(shader->shaderID);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    //SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(val);
}