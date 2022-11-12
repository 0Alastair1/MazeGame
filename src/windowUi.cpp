
static inline void initUi()
{
    init_SDL();
    init_ImGui();
}

static inline void init_SDL()
{
    #define windowHeight 720
    #define windowWidth 1080
    #define windowTitle "mazeGame"

    SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_LoadLibrary(NULL); 

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
	SDL_SetHint(SDL_HINT_RENDER_OPENGL_SHADERS, "1");
	SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "1");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN); //SDL_WINDOW_RESIZABLE
    if(window == NULL)
    {
        printf("ERROR: %s\n try running from commandline\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);

    //set sdl struct to window and context
    window = window;
    glContext = context;

    SDL_GL_MakeCurrent(window, context);

    // glad
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    glViewport(0, 0, windowHeight, windowWidth);

    SDL_ShowWindow(window);
    
    return;
}


static inline void init_ImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 400");
    ImGui::StyleColorsDark();
    return;
}