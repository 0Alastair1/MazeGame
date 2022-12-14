
/*void render() 
{
    //render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::string previousRenderObjectName = "";
    std::string previousShaderName = "";

    //view matrix - only needs to be ran once per frame
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), -mainCamera.position);

    //projection matrix
    glm::mat4 perspectiveProjectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 1.0f); //fixme

    for(gameToRenderObject* gameObject : gameToRenderObjects)
    {
        const renderObject* renderObj = gameObject->renderObj;

        //bind shader
        if(previousShaderName != renderObj->shader->shaderName)
            glUseProgram(renderObj->shader->shaderID); 

        //perform appropriate uniform operations depending on object shader and gameObject properties
        performUniformOperation(gameObject, viewMatrix, perspectiveProjectionMatrix);

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

    render_imgui();

    SDL_GL_SwapWindow(window);

    return;
}

static inline void performUniformOperation(const gameToRenderObject* gameObject, const glm::mat4 viewMatrix, const glm::mat4 perspectiveProjectionMatrix)
{
    const shaderObject* shaderObj = gameObject->renderObj->shader;
    
    if(!strcmp(shaderObj->shaderName.c_str(), "defaultShader")) //fixme dont hardcode in use forloop maybe not
    {
        //color uniform
        static float r = 0.0;
        static float increment = 0.001;
        if(r > 0.75)
            increment = -0.001;
        else if(r < 0.25)
            increment = 0.001;
        
        r += increment;

        //todo put matrix stuff in uniform object?

        //projection matrix
        glm::mat4 projectionMatrix;
    
        if(gameObject->orthoProj)
            projectionMatrix = mainCamera.orthoProjectionMatrix; //todo translate by camera rotation
        else
            projectionMatrix = perspectiveProjectionMatrix;
        

        //model projection 
        glm::mat4 modelPosition = glm::translate(glm::mat4(1.0f), gameObject->position);
        glm::mat4 modelRotation;
        glm::mat4 modelScale;

        glm::mat4 modelProj = modelPosition;

        //mvp - projection * view * model
        glm::mat4 mvp = projectionMatrix * viewMatrix * modelProj;
        
        //set uniforms
        //glUniformMatrix3x4fv
        glUniformMatrix4fv(shaderObj->u_mvpUniformLocation, 1, GL_FALSE, &mvp[0][0]);
        glUniform4f(shaderObj->u_colorUniformLocation, r+r/4, r-r/2, r+r/2, r+r/2);
    }
}*/

static inline void initRender()
{

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    const float ratio = (float)((float)windowwidth/(float)windowheight);
    const float scale = 700.0f; 
    mainCamera.orthoProjectionMatrix = glm::ortho(-windowwidth/scale, windowwidth/scale, -windowheight/scale, windowheight/scale, -1.0f, 100.0f );

    SDL_GL_SetSwapInterval(0); // no vysnc

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //enable these
    glCullFace(GL_BACK);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    genTextures();

    //create default shaders - todo add more
    setup_default_shaders();

    glUseProgram(getShader("defaultShader")->shaderID);

    //query gpu - todo later

    //vertex array object - this can stay here
    unsigned int VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glEnableVertexAttribArray(0); //the 0 corrasponds to the layout value in the shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, numberOfCollums * (sizeof(float)),(void*)0 );

    glEnableVertexAttribArray(1); //normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, numberOfCollums * (sizeof(float)),(void*)( 3*sizeof(float) ));

    glEnableVertexAttribArray(2); //texture coods
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, numberOfCollums * (sizeof(float)),(void*)( 6*sizeof(float) ));

    glEnableVertexAttribArray(3); //tangents
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, numberOfCollums * (sizeof(float)),(void*)( 8*sizeof(float) ));

    return;
}

static inline gameToRenderObject* makeGameObject(float* cobjectData, unsigned int* cindexData, Uint32 cverticies, Uint32 cindicies, const std::vector<std::string>& textureNames, bool orthoProject, bool batch, const glm::vec3 ambientCol, const glm::vec3 diffuseCol)
{
        
    gameToRenderObject* gameObject = new gameToRenderObject(cobjectData, cindexData, cverticies, cindicies, textureNames, orthoProject, ambientCol, diffuseCol);
    assignGameObjectToVertexBuffer(gameObject, batch);

    gameToRenderObjects.push_back(gameObject);
    return gameObject;
}

//hack
static inline gameToRenderObject* makeGameObject(std::vector<glm::vec3>& cobjectData, std::vector<Uint32>& cindexData, Uint32 cverticies, Uint32 cindicies, const std::vector<std::string>& textureNames, bool orthoProject, bool batch, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<glm::vec3>& tangents, const glm::vec3 ambientCol, const glm::vec3 diffuseCol)
{
    cverticies = cobjectData.size() * (sizeof(float) * numberOfCollums);

    float* objectData = (float*)malloc(cobjectData.size() * (sizeof(float) * numberOfCollums));
    unsigned int* indexData = (unsigned int*)malloc(cindexData.size() * (sizeof(unsigned int)));

    for (unsigned int i = 0; i < cobjectData.size(); i++)  
    {
        objectData[(i * numberOfCollums) + 0] = cobjectData[i].x;
        objectData[(i * numberOfCollums) + 1] = cobjectData[i].y;
        objectData[(i * numberOfCollums) + 2] = cobjectData[i].z;

        objectData[(i * numberOfCollums) + 3] = normals[i].x;
        objectData[(i * numberOfCollums) + 4] = normals[i].y;
        objectData[(i * numberOfCollums) + 5] = normals[i].z;

        objectData[(i * numberOfCollums) + 6] = texCoords[i].x;
        objectData[(i * numberOfCollums) + 7] = texCoords[i].y;

        if(tangents.size() == 0)
        {
            objectData[(i * numberOfCollums) + 8] = 0.0f;
            objectData[(i * numberOfCollums) + 9] = 0.0f;
            objectData[(i * numberOfCollums) +10] = 0.0f;
            continue;
        }

        objectData[(i * numberOfCollums) + 8] = tangents[i].x;
        objectData[(i * numberOfCollums) + 9] = tangents[i].y;
        objectData[(i * numberOfCollums) +10] = tangents[i].z;
    }

    for (unsigned int i = 0; i < cindexData.size(); i++)  
    {
        indexData[i] = cindexData[i];
    }

    return makeGameObject(objectData, indexData, cverticies, cindicies, textureNames, orthoProject, batch, ambientCol, diffuseCol);
}

/*
static inline void deleteGameObject(gameToRenderObject* gameObject) //fixme delete all references to that object from everhthing else
{
    for(long unsigned int i=0; i < gameToRenderObjects.size(); i++){
        if(gameToRenderObjects[i] == gameObject)
        {
            gameToRenderObjects.erase(gameToRenderObjects.begin() + i);
        }
    }
}
*/
