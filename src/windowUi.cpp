
static inline void initUi()
{
    init_SDL();
    init_ImGui();
}

#define windowWidth 1280
#define windowHeight 720   

static inline void init_SDL()
{
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

    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE); //SDL_WINDOW_RESIZABLE
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
    glViewport(0, 0, windowWidth, windowHeight);

    SDL_ShowWindow(window);

    SDL_GetWindowSize(window, &windowwidth, &windowheight);
    
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
static Uint32 drawCalls;
static inline void render_imgui()
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    ImGui::Begin("debug");
    ImGui::Text("%.3f ms/frame %.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("%d draw calls", drawCalls);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return;
}