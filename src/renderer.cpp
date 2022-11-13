

//8bit 640×400

void render() 
{
    //render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //test draw all render objects. *not actual game objects*
    for(renderObject* renderObject : renderObjects)
    {
        glUseProgram(renderObject->shader->shaderID); //bind shader

        performUniformOperation(renderObject->shader); //perform appropriate uniform operations depending on object shader

        //
        glBindTexture(GL_TEXTURE_2D, renderObject->texture->textureID); //bind texture


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

static inline void performUniformOperation(const shaderObject* program)
{
    if(!strcmp(program->shaderName, "defaultShader"))
    {
        static float r = 0.0;
        static float increment = 0.001;
        if(r > 0.75)
            increment = -0.001;
        else if(r < 0.25)
            increment = 0.001;
        
        r += increment;

        const int location = glGetUniformLocation(program->shaderID, "u_Color");//cache this fixme
        glUniform4f(location, r+r/4, r-r/2, r+r/2, r+r/2);
    }
}

static inline void initRender()
{

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    SDL_GL_SetSwapInterval(0); // no vysnc

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    genTextures();

    //create default  render objects
    setup_default_shaders();
    initRenderObjects();


    return;
}

static inline void initRenderObjects()
{
    const float triangleData[] = {
    -0.5f, -0.5f, 0.0f,/*color*/ 1.0f, 0.0f, 0.0f, /*tex coods*/
    0.5f, -0.5f, 0.0f, /*color*/ 0.0f, 1.0f, 0.0f, /*tex coods*/
    0.0f,  0.3f, 0.0f, /*color*/ 0.0f, 0.0f, 1.0f /*tex coods*/
    };
    const unsigned int numberofCollums = 6;
    const unsigned int triangleIndecies[] = {
        0, 1, 2
    };
    make_render_object_type(0, triangleData, sizeof(triangleData), numberofCollums, triangleIndecies, sizeof(triangleIndecies), "triangle", "defaultShader", "wood.png");
}

static inline void make_render_object_type(const Uint8 id, const float* vertexData, const unsigned int vertexDataSize, const unsigned int numberofCollums, const unsigned int* indices, const unsigned int indicesSize, const char* name, const char* shaderName, const char* textureName)
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

    glEnableVertexAttribArray(1); //color
    glVertexAttribPointer(1, (vertexDataSize/sizeof(float))/numberofCollums, GL_FLOAT, GL_FALSE, numberofCollums*(sizeof(float)),(void*)( 3*sizeof(float) ));

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
    object->vao = VertexArrayID;
    //maybe add index buffer here too - old intel driver bug
    //maybe add them all and not use vertex array - performance
    object->texture = getTexture(textureName);
    object->shader = getShader(shaderName);

    renderObjects.push_back(object);
}

static inline void setup_default_shaders()
{
    const char defaultVsSource[] =
    {
        "#version 330 core \n \
        layout(location = 0) in vec3 vertexPosition_modelspace; \
        layout(location = 1) in vec3 vColor; \
        \
        uniform vec4 u_Color; \
        \
        out vec4 fColor;\
        \
        void main()	\
        {	\
        	gl_Position.xyz = vertexPosition_modelspace;	\
        	gl_Position.w = 1.0;	\
            fColor = vec4(vColor, 1.0f) + u_Color; \
            \
        }"
    };

    const char defaultFsSource[] =
    {
        "#version 330 core \n    \
        layout(location = 0) out vec4 color;	\
        /*uniform vec4 u_Color;*/ \
        in vec4 fColor; \
        void main()	\
        {	\
        	color = fColor;\
        }"
    };

    makeShader(defaultVsSource, defaultFsSource, "defaultShader");
}

static inline void makeShader(const char* vertexSrc, const char* fragmentSrc, const char* shaderName)
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

    shaderObject* shader = new shaderObject;
    shader->shaderID = program;
    shader->shaderName = shaderName;
    shaderObjects.push_back(shader);
}


static inline void genTextures()
{
    const char* textureDirectory = "./textures/";
    const char* textureNames[] =
    {
        "wood.png",
    };

    stbi_set_flip_vertically_on_load(1);

    for(const char* textureName : textureNames)
    {
        char* filePath = (char*)malloc(strlen(textureDirectory) + strlen(textureName));
        strcpy(filePath,(char*)textureDirectory);
        strcat(filePath,(char*)textureName);

        int width = 0;
        int height = 0;
        int bpp = 0;
        unsigned char* buffer = stbi_load(filePath, &width, &height, &bpp, 4);
        free(filePath);

        unsigned int textureID = 0;

        glGenTextures(1, &textureID);

        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(buffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        textureObject* texture = new textureObject;
        texture->textureID = textureID;
        texture->textureName = textureName;
        textureObjects.push_back(texture);
    }
}

static inline textureObject* getTexture(const char* textureName)
{
    for(textureObject* texture : textureObjects)
    {
        if(!strcmp(texture->textureName, textureName))
        {
            return texture;
        }
    }
    printf("texture not found");
    return textureObjects[0];
}

static inline shaderObject* getShader(const char* shaderName)
{
    for(shaderObject* shader : shaderObjects)
    {
        if(!strcmp(shader->shaderName, shaderName))
        {
            return shader;
        }
    }
    printf("shader not found");
    return shaderObjects[0];
}