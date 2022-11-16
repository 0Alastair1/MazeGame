
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

    const float ratio = windowwidth/windowheight;
    const float scale = 700.0; 
    mainCamera.orthoProjectionMatrix = glm::ortho(-windowwidth/scale, windowwidth/scale, -windowheight/scale, windowheight/scale, -1.0f, 100.0f );

    SDL_GL_SetSwapInterval(0); // no vysnc

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    genTextures();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // increases stability for some reason

    //create default shaders - todo add more
    setup_default_shaders();

    glUseProgram(getShader("defaultShader")->shaderID);

    //query gpu - todo later

    //vertex array object - this can stay here
    unsigned int VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glEnableVertexAttribArray(0); //the 0 corrasponds to the layout value in the shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9*(sizeof(float)),(void*)0 );

    glEnableVertexAttribArray(1); //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9*(sizeof(float)),(void*)( 3*sizeof(float) ));

    glEnableVertexAttribArray(2); //texture coods
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9*(sizeof(float)),(void*)( 6*sizeof(float) ));

    glEnableVertexAttribArray(3); //texture index
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9*(sizeof(float)),(void*)( 8*sizeof(float) ));

    return;
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