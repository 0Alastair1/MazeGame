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