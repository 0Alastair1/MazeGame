static inline void setup_default_shaders()
{
    const char defaultVsSource[] =
    {
        "#version 330 core \n \
        layout(location = 0) in vec3 vertexes; \
        layout(location = 1) in vec3 normals; \
        layout(location = 2) in vec2 vTexCord; \
        layout(location = 3) in vec3 tangents; \
        \
        uniform vec4 u_Color; \
        uniform mat4 vp; \
        \
        out vec4 fColor;\
        out vec2 texCord; \
        out mat3 tbn; \
        \
        void main()	\
        {	\
        	gl_Position = vp * vec4(vertexes, 1.0);	\
            fColor = u_Color; \
            texCord = vTexCord; \
            \
            vec3 tangent = normalize(vec3(vp * vec4(tangents, 0.0)));\
            vec3 normal = normalize(vec3(vp * vec4(normals, 0.0)));\
            tangent = normalize(tangent - dot(tangent, normal) * normal);\
            vec3 biTangent = cross(normal, tangent);\
            tbn = mat3(tangent, biTangent, normal);\
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
        in mat3 tbn; \
        \
        uniform sampler2D TextureSlot0; /*diffuse*/ \
        uniform sampler2D TextureSlot1; /*normal*/ \
        uniform sampler2D TextureSlot2; /*specular*/ \
        uniform sampler2D TextureSlot3; /*height*/ \
        uniform sampler2D TextureSlot4; /*ambient*/ \
        uniform sampler2D TextureSlot5; /*light*/ \
        uniform sampler2D TextureSlot6; /*emissive*/ \
        uniform sampler2D TextureSlot7; /*shininess*/ \
        uniform sampler2D TextureSlot8; /*clearcoat*/ \
        uniform sampler2D TextureSlot9; /*metalness*/ \
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

    //get locations from shader- generic
    for (size_t i = 0; i < 32; i++)
    {
        const char* textureSlotName = "TextureSlot";
        std::stringstream ss;
        ss << i;
        std::string slotNumber = ss.str().c_str();
        std::string textureSlotFullName = textureSlotName + slotNumber;

        shader->textureUniformLocations[i] = glGetUniformLocation(program, textureSlotFullName.c_str()); //todo make this func a std::String
        if(shader->textureUniformLocations[i] == -1)
            break;
        glUniform1i(shader->textureUniformLocations[i], i);
    }

    shader->u_colorUniformLocation = glGetUniformLocation(program, "u_Color");
    shader->u_mvpUniformLocation = glGetUniformLocation(program, "vp");

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