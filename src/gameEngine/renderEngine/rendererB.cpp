
static inline void render() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //view matrix - only needs to be ran once per frame
    mainCamera.perspectiveProjectionMatrix = glm::perspective(45.0f, (float)windowwidth / (float)windowheight, 0.1f, 100.0f);

    //projection matrix

    mainCamera.cameraFacingDirVec = glm::normalize(yawPitchDirectionCalc(mainCamera.yaw, mainCamera.pitch));

    glm::mat4 viewMatrix = glm::lookAt(mainCamera.position, mainCamera.position +  mainCamera.cameraFacingDirVec, mainCamera.cameraUp);

    drawCalls = 0;
    for(vertexBufferStruct* vbs : vertexBuffers)
    {
        //bind shader - to do optimize
        glUseProgram(vbs->shader->shaderID);

        performUniformOperation(vbs, viewMatrix);

        size_t gameObjectIndex = 0;
        const float* vertexDest =  vbs->vertexData;
        const Uint32 eachVertexSize = vbs->eachVertexSize;
        
        for(gameToRenderObject* gameObject : vbs->bindedGameObjects)
        {
            if(gameObject->verticiesChanged == false)
                continue;
            
            memcpy((void*)(vertexDest + (gameObjectIndex * (eachVertexSize/sizeof(float)))), gameObject->viData->objectData, eachVertexSize);

            gameObjectIndex++;

            //testing
            /*
            printf("\n\n");
            printf("object %d ", gameObjectIndex);
            printf("\n\n");
            for(size_t i = 0; i < eachVertexSize / sizeof(float); i++)
            {
                if(i % 9 == 0)
                {
                    printf("\n");
                }
                printf("%f , ", gameObject->viData->objectData[i]);
            }*/
        }
        printf("\n");
        printf("vbs data:         ");
        printf("\n");
        for(size_t i = 0; i < vbs->bindedGameObjects.size() * eachVertexSize/ sizeof(float); i++)
        {
            if(i % 9 == 0)
            {
                printf("\n");
            }
            printf("%f , ", vertexDest[i]);
        }

        //bind textures - todo optimize this

        for(size_t i = 0; i < vbs->texturesBinded.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); 
            glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
        }
        /*
        for(size_t i = 0; i < 8; i++)
        {
            glBindTexture(i, vbs->texturesBinded[i]->textureID);
        }*/

        //bind the buffers
        glBindBuffer(GL_ARRAY_BUFFER, vbs->vertexbuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vbs->fullVertexDataSize, vertexDest);//suppose to be vbs->bindedGameObjects.size() * eachVertexSize??

        //the 0 corrasponds to the layout value in the shader
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9*(sizeof(float)),(void*)0 );

       //color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9*(sizeof(float)),(void*)( 3*sizeof(float) ));

        //texture coods
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9*(sizeof(float)),(void*)( 6*sizeof(float) ));

        //texture index
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9*(sizeof(float)),(void*)( 8*sizeof(float) ));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbs->indexbuffer);


        drawCalls++;
        glDrawElements(GL_TRIANGLES, vbs->fullNumberOfElements, GL_UNSIGNED_INT, nullptr);
    }

    render_imgui();

    SDL_GL_SwapWindow(window);
}

static inline void performUniformOperation(const vertexBufferStruct* vbs, const glm::mat4 viewMatrix)
{
    
    if(!strcmp(vbs->shader->shaderName.c_str(), "defaultShader")) //fixme dont hardcode in use forloop maybe not
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
    
        if(vbs->orthoProj)
            projectionMatrix = mainCamera.orthoProjectionMatrix; //todo translate by camera rotation
        else
            projectionMatrix = mainCamera.perspectiveProjectionMatrix;
        

        //mvp - projection * view * model
        glm::mat4 vp = projectionMatrix * viewMatrix;
        
        //set uniforms
        //glUniformMatrix3x4fv
        glUniformMatrix4fv(vbs->shader->u_mvpUniformLocation, 1, GL_FALSE, &vp[0][0]);
        glUniform4f(vbs->shader->u_colorUniformLocation, r+r/4, r-r/2, r+r/2, r+r/2);
    }
}

static inline void assignGameObjectToVertexBuffer(gameToRenderObject* gameObject)
{

    //get id of the object's vertex and index buffer
   Uint32 i = 0;
   bool found = false;
   for(verticesindexesData* viDataTaken : uniqueViData)
   {
        if(viDataTaken->verticies == gameObject->viData->verticies && viDataTaken->indicies == gameObject->viData->indicies)
        {
            if(memcmp(viDataTaken->objectData, gameObject->viData->objectData, gameObject->viData->verticies) == 0)
            {
                if(memcmp(viDataTaken->indexData, gameObject->viData->indexData, gameObject->viData->indicies) == 0)
                {
                    found = true;
                    break;
                }
            }
        }
        i++;
   }
   if(!found)
   {    //make a copy of original object data
        verticesindexesData* vid = new verticesindexesData;
        vid->objectData = (float*)malloc(1000 * sizeof(gameObject->viData->verticies));
        vid->indexData = (unsigned int*)malloc(1000 * sizeof(gameObject->viData->indicies));
        vid->verticies = gameObject->viData->verticies;
        vid->indicies = gameObject->viData->indicies;

        memcpy((void*)vid->objectData, gameObject->viData->objectData, sizeof(gameObject->viData->verticies));
        memcpy((void*)vid->indexData, gameObject->viData->indexData, sizeof(gameObject->viData->indicies));

        uniqueViData.push_back(gameObject->viData);
   }

    const Uint32 viIndex1 = i;

    //serach available vertex buffers objects to use

    //check vertex buffer type and objects amount binded
    std::vector<vertexBufferStruct*> possiblevbs;    
    for(vertexBufferStruct* vbs : vertexBuffers)
    {
        if(vbs->viIndex == viIndex1 && vbs->bindedGameObjects.size() < 1000) //replace 1000 with vram size minus a bit? how to get vram size?
        {
            possiblevbs.push_back(vbs);
        }
    }

    //search for vbs with existing binded texture - piortize
    std::vector<vertexBufferStruct*> possiblevbs2; 
    for(vertexBufferStruct* vbs : possiblevbs)
    {
        size_t textureIndex = 0;
        for(textureObject* to : vbs->texturesBinded)
        {
            if(to == gameObject->to)
            {
                //same vi type, not at binded objects limit, and got binded texture already
                updateTextureBinding(textureIndex, gameObject);
                vbs->bindedGameObjects.push_back(gameObject);
                return;
            }
            textureIndex++;
        }   
    }

    //no texture already binded, can bind new check
    for(vertexBufferStruct* vbs : possiblevbs)
    {
        if(vbs->texturesBinded.size() < 8) //todo set to device max texture bind later.
        {
            //bind texture
            updateTextureBinding(vbs->texturesBinded.size(), gameObject);
            vbs->texturesBinded.push_back(gameObject->to); //todo at some point make able to bind multiple textures per object

            vbs->bindedGameObjects.push_back(gameObject);
            return;
        }
    }

    //seems like no suitable vertex buffer was found, create a new one

    vertexBufferStruct* vbs = new vertexBufferStruct;
    vertexBuffers.push_back(vbs);

    vbs->viIndex = viIndex1;
    vbs->bindedGameObjects.push_back(gameObject);

    //bind shaders
    vbs->texturesBinded.push_back(gameObject->to);
    updateTextureBinding(0, gameObject);

    //vertex buffer object
    glGenBuffers(1, &vbs->vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vbs->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, 1000 * gameObject->viData->verticies, nullptr, GL_DYNAMIC_DRAW);


    //index buffer object - autogen for future use

    const Uint32 indexBufferSize = gameObject->viData->indicies;
    const Uint32 allIndexindexBufferSizes = 1000 * indexBufferSize;

    vbs->fullIndexDataSize = allIndexindexBufferSizes;
    vbs->indexData = (Uint32*)malloc(allIndexindexBufferSizes);
    vbs->fullNumberOfElements = vbs->fullIndexDataSize/sizeof(Uint32);
    //vbs->shader = gameObject-> fix shader per object
    vbs->orthoProj = gameObject->orthoProj;

    vbs->fullVertexDataSize = 1000 * gameObject->viData->verticies;
    vbs->eachVertexSize = gameObject->viData->verticies;
    vbs->vertexData = (float*)malloc(vbs->fullVertexDataSize);

    const Uint32* srcData = gameObject->viData->indexData;
    const Uint32* dstData = vbs->indexData;

    for (size_t ii = 0; ii < 1000; ii++)
    {
        memcpy((void*)((char*)dstData + (ii * indexBufferSize)), srcData, indexBufferSize);
    }
    
    glGenBuffers(1, &vbs->indexbuffer);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbs->indexbuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, allIndexindexBufferSizes, dstData, GL_STATIC_DRAW);

    /*
    //testing
    for(size_t ij = 0; ij < vbs->fullNumberOfElements; ij++)
    {
        printf("%d ",dstData[ij]);
    }*/
}

static inline void updateTextureBinding(Uint8 textureIndex, gameToRenderObject* gameObject) //texture index of the vbs - todo make changing textures not in vbs
{
    gameObject->bindedTextureSlot = textureIndex;
    const float texureIndexFloat = textureIndex;

    for(size_t i =0; i < ((gameObject->viData->verticies/4)/3)/3; i++)
    {
        memcpy((void*)&gameObject->viData->objectData[(i * 9) + 8], &texureIndexFloat, sizeof(float));
    }
}

static inline glm::vec3 yawPitchDirectionCalc(float yaw, float pitch)
{
    const glm::vec3 direction (cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)),sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    return direction;
}