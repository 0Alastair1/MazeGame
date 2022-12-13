static inline void setup_default_shaders()
{
    makeShader(defaultVsSource, defaultFsSource, "defaultShader");
    makeShader(testVsSource, testFsSource, "testShader");
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

    //get locations from shader- generic
    for (size_t i = 0; i < 32; i++) //i increments on each iteration
    {
        const char* textureSlotName = "TextureSlot";
        std::stringstream ss;
        ss << i;
        std::string slotNumber = ss.str().c_str();
        std::string textureSlotFullName = textureSlotName + slotNumber;

        shader->textureUniformLocations[i] = glGetUniformLocation(program, textureSlotFullName.c_str()); //todo make this func a std::String
        if(shader->textureUniformLocations[i] == -1)
            break;
        //glUniform1i(shader->textureUniformLocations[i], i);
    }

    shader->u_colorUniformLocation = glGetUniformLocation(program, "u_Color");
    shader->u_mvpUniformLocation = glGetUniformLocation(program, "vp");
    shader->u_ambientLightColLocation = glGetUniformLocation(program, "ambientLightCol");
    shader->u_ambientLightIntensityLocation = glGetUniformLocation(program, "ambientLightIntensity");
    shader->u_matAmbientLightColLocation = glGetUniformLocation(program, "matAmbientLightCol");
    shader->uDirectionLightLocLocation = glGetUniformLocation(program, "uDirectionLightLoc");
    shader->uDirectionLightIntensityLocation = glGetUniformLocation(program, "uDirectionLightIntensity");
    shader->uDiffuseLightColLocation = glGetUniformLocation(program, "diffuseLightCol");

    //non generic uniforms
    const char* index = vertexSrc;
    const char* indexSquash = vertexSrc;
    while(index != nullptr)
    {
        index = strstr(indexSquash, "uniform");
        if(index == nullptr)
            break;
        index += 13;
        indexSquash = strstr(index, ";");

        const char* uniformName = (const char*)malloc(indexSquash - index);
        memcpy((void*)uniformName, index, indexSquash - index);
        shader->nonGenericUniformlocations[uniformName] = glGetUniformLocation(program, uniformName);
        free((void*)uniformName);
    }

    shaderObjects.push_back(shader);
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