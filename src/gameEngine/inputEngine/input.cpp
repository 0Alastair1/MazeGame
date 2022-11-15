static inline void updateInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) 
        {
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SDL_GetWindowSize(window, &windowwidth, &windowheight);
                    glViewport(0, 0, windowwidth, windowheight);

                    const float ratio = windowwidth/windowheight;
                    const float scale = 700.0; 
                    mainCamera.orthoProjectionMatrix = glm::ortho(-windowwidth/scale, windowwidth/scale, -windowheight/scale, windowheight/scale, -1.0f, 100.0f );
                }
                break;

            case SDL_QUIT:
                program_exit(0);
                break;
            

            default:
                break;
        }
    }
}