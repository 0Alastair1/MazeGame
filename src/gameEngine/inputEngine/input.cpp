static inline void updateInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) 
        {
            case SDL_MOUSEMOTION:
				mainCamera.yaw += event.motion.xrel * 0.05f;
				mainCamera.pitch -= event.motion.yrel * 0.05f;
				if (mainCamera.pitch > 89.0f) mainCamera.pitch = 89.0f;
				if (mainCamera.pitch < -89.0f ) mainCamera.pitch = -89.0f;
				break;

            case SDL_KEYDOWN:
                updateKeyboardKey(&event.key, true);
                break;

            case SDL_KEYUP:
                updateKeyboardKey(&event.key, false);
                break;

            /*
            case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
                    
                    case SDL_BUTTON_LEFT:
                        
                        break;
                    case SDL_BUTTON_RIGHT:
                        break;
                    
				break;
                */

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SDL_GetWindowSize(window, &windowwidth, &windowheight);
                    glViewport(0, 0, windowwidth, windowheight);

                    const float ratio = windowwidth/windowheight;
                    const float scale = 700.0; 
                    mainCamera.orthoProjectionMatrix = glm::ortho(-windowwidth/scale, windowwidth/scale, -windowheight/scale, windowheight/scale, -1.0f, 100.0f );

                    mainCamera.perspectiveProjectionMatrix = glm::perspective(45.0f, (float)windowwidth / (float)windowheight, 0.1f, 100.0f);
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

static bool keys[255];

static inline void updateKeyboardKey(SDL_KeyboardEvent* key, bool press)
{
    //std::cout << (char)key->keysym.sym << std::endl;
    keys[key->keysym.sym] = press;
}

static inline bool getKey(char key)
{
    return keys[(Uint8)key];
}