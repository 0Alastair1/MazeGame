

//8bit 640×400

void render() 
{
    //render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float r = 0.0;
    static float increment = 0.001;
    if(r > 0.75)
        increment = -0.001;
    else if(r < 0.25)
        increment = 0.001;
    
    r += increment;

    //figure out what to do here
    //int location = glGetUniformLocation(defaultShader, "u_Color");
    glUniform4f(0, r+r/4, r-r/2, r+r/2, r+r/2);

    //test draw all render objects. *not actual game objects*
    for(renderObject* renderObject : renderObjects)
    {
        glUseProgram(renderObject->program);
        glBindVertexArray(renderObject->vao);
        glDrawElements(GL_TRIANGLES, renderObject->indicies, GL_UNSIGNED_INT, nullptr);
    }

    /*
        todo - leave the added of objects sorted by the id the renderobject uses to the game logic side
        in render keep track of the previsouly used renderobject and only rebind everything if it changed
    */

    SDL_GL_SwapWindow(window);

    return;
}


void initRender()
{

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    SDL_GL_SetSwapInterval(0); // no vysnc

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //create default  render objects
    const unsigned int defaultShader = setup_default_shaders();
    initRenderObjects(defaultShader);

    genTextures();


    return;
}

static inline void initRenderObjects(const unsigned int defaultShader)
{
    const float triangleData[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.3f, 0.0f,
    };
    const unsigned int numberofCollums = 3;
    const unsigned int triangleIndecies[] = {
        0, 1, 2
    };
    make_render_object_type(0, triangleData, sizeof(triangleData), numberofCollums, triangleIndecies, sizeof(triangleIndecies), "triangle", defaultShader);
}

static inline void make_render_object_type(const Uint8 id, const float* vertexData, const unsigned int vertexDataSize, const unsigned int numberofCollums, const unsigned int* indices, const unsigned int indicesSize, const char* name, const unsigned int shader)
{
    //vertex array object
    unsigned int VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //vertex buffer object
    unsigned int vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); //the 0 corrasponds to the layout value in the shader
    glVertexAttribPointer(0, (vertexDataSize/sizeof(float))/numberofCollums, GL_FLOAT, GL_FALSE, numberofCollums*(sizeof(float)),(void*)0 );

    //index buffer object
    unsigned int indexbuffer;
    glGenBuffers(1, &indexbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &indexbuffer);
    glDisableVertexAttribArray(1);

    renderObject* object = new renderObject;
    object->id = id;
    object->indicies = indicesSize/sizeof(unsigned int);
    object->name = name;
    object->program = shader;
    object->vao = VertexArrayID;
    //maybe add index buffer here too - old intel driver bug
    //maybe add them all and not use vertex array - performance

    renderObjects.push_back(object);
}

static inline unsigned int setup_default_shaders()
{
    const char defaultVsSource[] =
    {
        "#version 330 core \n	\
        layout(location = 0) in vec3 vertexPosition_modelspace; \
        uniform vec4 u_Color; \
        void main()	\
        {	\
        	gl_Position.xyz = vertexPosition_modelspace;	\
        	gl_Position.w = 1.0;	\
        }"
    };

    const char defaultFsSource[] =
    {
        "#version 330 core \n    \
        layout(location = 0) out vec4 color;	\
        uniform vec4 u_Color; \
        void main()	\
        {	\
        	color = u_Color;	\
        }"
    };

    return makeShader(defaultVsSource, defaultFsSource);
}

static inline unsigned int makeShader(const char* vertexSrc, const char* fragmentSrc)
{
    GLuint defaultVs = glCreateShader( GL_VERTEX_SHADER);

    glShaderSource(defaultVs, 1, &vertexSrc, NULL);
    glCompileShader(defaultVs );

    GLint Result = GL_FALSE;
	int InfoLogLength;

    glGetShaderiv(defaultVs, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(defaultVs, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(defaultVs, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}


    GLuint defaultFs = glCreateShader( GL_FRAGMENT_SHADER );

    glShaderSource(defaultFs, 1, &fragmentSrc, NULL);
    glCompileShader(defaultFs);

    glGetShaderiv(defaultFs, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(defaultFs, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(defaultFs, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

    unsigned int program = glCreateProgram();

    glAttachShader(program, defaultVs);
    glAttachShader(program, defaultFs);

    glLinkProgram(program);

    glDetachShader(program, defaultVs);
    glDetachShader(program, defaultFs);

    glDeleteShader(defaultVs);
    glDeleteShader(defaultFs);

    //default
    return program;
}


static inline void genTextures()
{
    const char* textureDirectory = "";
    const char* textureNames[] =
    {
        "wood",
    };

    for(const char* textureName : textureNames)
    {

    }
    stbi_set_flip_vertically_on_load(1);
   //unsigned char* buffer = stbi_load("")
}