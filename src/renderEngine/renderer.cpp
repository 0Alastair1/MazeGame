
void render() 
{
    //render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::string previousRenderObjectName = "";
    std::string previousShaderName = "";

    for(gameToRenderObject* gameObject : gameToRenderObjects)
    {
        const renderObject* renderObj = gameObject->renderObj;

        //bind shader
        if(previousShaderName != renderObj->shader->shaderName)
            glUseProgram(renderObj->shader->shaderID); 

        //perform appropriate uniform operations depending on object shader and gameObject properties
        performUniformOperation(gameObject);

        if(previousRenderObjectName != renderObj->name)
        {
            //bind textures
            Uint8 textureIndex = 0;
            const Sint32* textureUniformLocations = renderObj->shader->textureUniformLocations;
            for(auto texture : renderObj->textures) //bind all the textures to shader
            {
                glActiveTexture(GL_TEXTURE0 + textureIndex); //bind to slot 0
                glBindTexture(GL_TEXTURE_2D, texture->textureID); //bind texture
                glUniform1i(textureUniformLocations[textureIndex], textureIndex);
                textureIndex++;
            }    

            //bind vao
            glBindVertexArray(renderObj->vao);
        }    

        //draw
        glDrawElements(GL_TRIANGLES, renderObj->indicies, GL_UNSIGNED_INT, nullptr);

        previousShaderName = renderObj->shader->shaderName;
        previousRenderObjectName = renderObj->name;
    }

    SDL_GL_SwapWindow(window);

    return;
}

static inline void performUniformOperation(const gameToRenderObject* gameObject)
{
    if(!strcmp(gameObject->renderObj->shader->shaderName.c_str(), "defaultShader")) //fixme dont hardcode in use forloop
    {
        static float r = 0.0;
        static float increment = 0.001;
        if(r > 0.75)
            increment = -0.001;
        else if(r < 0.25)
            increment = 0.001;
        
        r += increment;

        const int location = glGetUniformLocation(gameObject->renderObj->shader->shaderID, "u_Color");//cache this fixme
        glUniform4f(location, r+r/4, r-r/2, r+r/2, r+r/2);
    }
}

static inline void initRender()
{

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    SDL_GL_SetSwapInterval(0); // no vysnc

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    genTextures();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // increases stability for some reason
    //create default  render objects
    setup_default_shaders();
    initRenderObjects();

    return;
}

static inline void initRenderObjects() //default objects
{
    const float triangleData[] = {
    -0.5f, -0.5f, 0.0f,/*color*/ 1.0f, 0.0f, 0.0f, /*tex cords*/ -1.0f, -1.0f,
    0.5f, -0.5f, 0.0f, /*color*/ 0.0f, 1.0f, 0.0f, /*tex cords*/  1.0f, -1.0f,
    0.5f,  0.3f, 0.0f, /*color*/ 0.0f, 0.0f, 1.0f, /*tex cords*/  1.0f, 1.0f,
    -0.5f,  0.3f, 0.0f,/*color*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ -1.0f, 1.0f
    };
    const unsigned int numberofCollums = 8;
    const unsigned int triangleIndecies[] = {
        0, 1, 2,
        0, 2, 3
    };
    make_render_object_type(0, triangleData, sizeof(triangleData), numberofCollums, triangleIndecies, sizeof(triangleIndecies), 
        "square", "defaultShader", {"wood.png"}
    );
}

static inline void make_render_object_type(const Uint8 id, const float* vertexData, const unsigned int vertexDataSize, const unsigned int numberofCollums, const unsigned int* indices, const unsigned int indicesSize, const char* name, const char* shaderName, std::vector<const char*> textureNames)
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, numberofCollums*(sizeof(float)),(void*)0 );

    glEnableVertexAttribArray(1); //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, numberofCollums*(sizeof(float)),(void*)( 3*sizeof(float) ));

    glEnableVertexAttribArray(2); //texture coods
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, numberofCollums*(sizeof(float)),(void*)( 6*sizeof(float) ));

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
    for(auto textureName : textureNames)
    {
        object->textures.push_back(getTexture(textureName));
    }
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
        layout(location = 2) in vec2 vTexCord; \
        \
        uniform vec4 u_Color; \
        \
        out vec4 fColor;\
        out vec2 texCord; \
        \
        void main()	\
        {	\
        	gl_Position.xyz = vertexPosition_modelspace;	\
        	gl_Position.w = 1.0;	\
            fColor = vec4(vColor, 1.0f) + u_Color; \
            texCord = vTexCord; \
            \
        }"
    };

    const char defaultFsSource[] =
    {
        "#version 330 core \n    \
        layout(location = 0) out vec4 color;	\
        \
        /*uniform vec4 u_Color;*/ \
        \
        in vec4 fColor; \
        in vec2 texCord; \
        \
        uniform sampler2D TextureSlot0; \
        uniform sampler2D TextureSlot1; \
        uniform sampler2D TextureSlot2; \
        uniform sampler2D TextureSlot3; \
        uniform sampler2D TextureSlot4; \
        uniform sampler2D TextureSlot5; \
        uniform sampler2D TextureSlot6; \
        uniform sampler2D TextureSlot7; \
        uniform sampler2D TextureSlot8; \
        \
        void main()	\
        {	\
        	color = texture(TextureSlot0, texCord) * fColor; \
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

    //get locations from shader
    shader->textureUniformLocations[0] = glGetUniformLocation(program, "TextureSlot0");
    shader->textureUniformLocations[1] = glGetUniformLocation(program, "TextureSlot1");
    shader->textureUniformLocations[2] = glGetUniformLocation(program, "TextureSlot2");
    shader->textureUniformLocations[3] = glGetUniformLocation(program, "TextureSlot3");
    shader->textureUniformLocations[4] = glGetUniformLocation(program, "TextureSlot4");
    shader->textureUniformLocations[5] = glGetUniformLocation(program, "TextureSlot5");
    shader->textureUniformLocations[6] = glGetUniformLocation(program, "TextureSlot6");
    shader->textureUniformLocations[7] = glGetUniformLocation(program, "TextureSlot7");
    shader->textureUniformLocations[8] = glGetUniformLocation(program, "TextureSlot8");

    shaderObjects.push_back(shader);
}


static inline void genTextures() //textures are made automaticlly when added into the textures folder
{
    boost::filesystem::path aaa = boost::dll::program_location().parent_path();
    const char* currentDir = aaa.string().c_str();
    const char* texturesFolder = "/textures/";
    char* textureDirectory = (char*)malloc(strlen(currentDir) + strlen(texturesFolder));
    strcpy(textureDirectory,(char*)currentDir);
    strcat(textureDirectory,(char*)texturesFolder);

    std::vector<const char*> textureNames;

    for (const auto & entry : std::filesystem::directory_iterator(textureDirectory))
    {
        if(std::filesystem::is_directory(entry.path()))
        {
            continue;
        }

        textureNames.push_back(std::filesystem::path{entry.path()}.filename().string().c_str());
    }

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
        if(!strcmp(texture->textureName.c_str(), textureName))
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
        if(!strcmp(shader->shaderName.c_str(), shaderName))
        {
            return shader;
        }
    }
    printf("shader not found");
    return shaderObjects[0];
}

static inline renderObject* getRenderObject(const char* renderObjectName)
{
    for(renderObject* renderObj : renderObjects)
    {
        if(!strcmp(renderObj->name.c_str(), renderObjectName))
        {
            return renderObj;
        }
    }
    printf("render object not found");
    return renderObjects[0];
}

static inline gameToRenderObject* makeGameObject()
{
    gameToRenderObject* gameObject = new gameToRenderObject;

    gameToRenderObjects.push_back(gameObject);

    return gameObject;
}

static inline void deleteGameObject(gameToRenderObject* gameObject)
{
    for(long unsigned int i=0; i < gameToRenderObjects.size(); i++){
        if(gameToRenderObjects[i] == gameObject)
        {
            gameToRenderObjects.erase(gameToRenderObjects.begin() + i);
        }
    }
}