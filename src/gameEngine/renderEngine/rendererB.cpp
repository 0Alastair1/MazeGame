static Sint32 prevShader = -1;
static Uint32 texturesBindedprev[32];
static const Uint32 maxVertPerVertexBuffer = 10000 * 4;

static inline void render() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //view matrix - only needs to be ran when window resized oor fov change
    mainCamera.perspectiveProjectionMatrix = glm::perspective(45.0f, (float)windowwidth / (float)windowheight, 0.1f, 10000.0f); //remove this

    //projection matrix

    mainCamera.cameraFacingDirVec = glm::normalize(yawPitchDirectionCalc(mainCamera.yaw, mainCamera.pitch));

    glm::mat4 viewMatrix = glm::lookAt(mainCamera.position, mainCamera.position +  mainCamera.cameraFacingDirVec, mainCamera.cameraUp);

    uniFormPerFrame();

    drawCalls = 0;
    for(vertexBufferStruct* vbs : vertexBuffers)
    {
        //bind shader - to do optimize
        if(vbs->shader->shaderID != prevShader)
        {
            glUseProgram(vbs->shader->shaderID);
            prevShader = vbs->shader->shaderID;
        }

        performUniformOperation(vbs, viewMatrix);

        size_t gameObjectIndex = 0;
        const float* vertexDest =  vbs->vertexData;
        const Uint32 eachVertexSize = vbs->eachVertexSize;
        
        for(gameToRenderObject* gameObject : vbs->bindedGameObjects)
        {
            if(gameObject->verticiesChanged == false)
                continue;
            
            gameObject->verticiesChanged = false;
            memcpy((void*)(vertexDest + (gameObjectIndex * (eachVertexSize/sizeof(float)))), gameObject->viData->objectData, eachVertexSize);

            gameObjectIndex++;
        }

        //bind textures - todo optimize this

        for(size_t i = 0; i < vbs->texturesBinded.size(); i++)
        {
            /*if(texturesBindedprev[i] != vbs->texturesBinded[i]->textureID)
            {            
                glActiveTexture(GL_TEXTURE0 + i); 
                glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                //glUniform1i(i, vbs->texturesBinded[i]->textureID);
                texturesBindedprev[i] = vbs->texturesBinded[i]->textureID;
            }*/
            switch(vbs->texturesBinded[i]->type) //to do multiple textures of same type
            {
                case textureTypeEnum::diffuse:
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[0], 0);
                    break;

                case textureTypeEnum::normal:
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[1], 1);
                    break;

                case textureTypeEnum::specular:
                    glActiveTexture(GL_TEXTURE2);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[2], 2);
                    break;

                case textureTypeEnum::height:
                    glActiveTexture(GL_TEXTURE3);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[3], 3);
                    break;

                case textureTypeEnum::ambient:
                    glActiveTexture(GL_TEXTURE4);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[4], 4);    
                    break;

                case textureTypeEnum::light:
                    glActiveTexture(GL_TEXTURE5);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[5], 5);
                    break;

                case textureTypeEnum::emissive:
                    glActiveTexture(GL_TEXTURE6);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[6], 6);
                    break;
                    
                case textureTypeEnum::shininess:
                    glActiveTexture(GL_TEXTURE7);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[7], 7);
                    break;

                case textureTypeEnum::clearcoat:
                    glActiveTexture(GL_TEXTURE8);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[8], 8);
                    break;

                case textureTypeEnum::metalness:
                    glActiveTexture(GL_TEXTURE9);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[9], 9);
                    break;
            }
        }

        //bind the buffers
        glBindBuffer(GL_ARRAY_BUFFER, vbs->vertexbuffer);
        //glBufferSubData(GL_ARRAY_BUFFER, 0, vbs->fullVertexDataSize, vertexDest);//suppose to be vbs->bindedGameObjects.size() * eachVertexSize??
        glBufferSubData(GL_ARRAY_BUFFER, 0, vbs->bindedGameObjects.size() * eachVertexSize, vertexDest);

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

static inline void uniFormPerFrame()
{
    //color uniform

    /*
    for(int i = 0; i < (int)deltaTime/3; i++) //remove
    {
        if(color > 0.60) colorIncrease = -0.001f;
        else if(color < 0.40) colorIncrease = 0.001f;
    
        color += colorIncrease;
    }*/

}

static inline void performUniformOperation(const vertexBufferStruct* vbs, const glm::mat4 viewMatrix)
{
    
    if(!strcmp(vbs->shader->shaderName.c_str(), "defaultShader")) //fixme dont hardcode in use forloop maybe not
    {

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
        glUniform4f(vbs->shader->u_colorUniformLocation, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

static inline void assignGameObjectToVertexBuffer(gameToRenderObject* gameObject, const bool batch)
{                          //todo change amount of objects per vbs to depend on the number of vertices
    Sint32 viIndex1 = -1;
    if(batch)
    {
        //get id of the object's vertex and index buffer
        Uint32 i = 0;
        bool found = false;
        for(verticesindexesData* viDataTaken : uniqueViData)
        {
                if(viDataTaken->verticies == gameObject->viData->verticies && viDataTaken->indicies == gameObject->viData->indicies)
                {
                    if(memcmp(viDataTaken->indexData, gameObject->viData->indexData, gameObject->viData->indicies) == 0)
                    {
                        found = true;
                        break;
                    }
                }
                i++;
        }
        if(!found)
        {    //make a copy of original object data
                verticesindexesData* vid = new verticesindexesData;
                vid->objectData = (float*)malloc(gameObject->viData->verticies);
                vid->indexData = (unsigned int*)malloc(gameObject->viData->indicies);
                vid->verticies = gameObject->viData->verticies;
                vid->indicies = gameObject->viData->indicies;

                memcpy((void*)vid->objectData, gameObject->viData->objectData,gameObject->viData->verticies);
                memcpy((void*)vid->indexData, gameObject->viData->indexData, gameObject->viData->indicies);

                uniqueViData.push_back(vid);
        }

        viIndex1 = i;

        //serach available vertex buffers objects to use

        //check vertex buffer type and objects amount binded
        std::vector<vertexBufferStruct*> possiblevbs;    
        for(vertexBufferStruct* vbs : vertexBuffers)
        {
            if(vbs->viIndex == viIndex1 && vbs->bindedGameObjects.size() < vbs->maxObjects && vbs->noBatch == false)
            {
                possiblevbs.push_back(vbs);
            }
        }

        //search for vbs with existing binded texture - piortize
        std::vector<vertexBufferStruct*> possiblevbs2; 
        for(vertexBufferStruct* vbs : possiblevbs)
        {
            std::vector<textureObject*>textureBindsIds;
            Uint8 texturesFound = 0;
            for(size_t ii = 0; ii < vbs->texturesBinded.size(); ii++)
            {
                for(textureObject* to : gameObject->to)
                {
                    if(to == vbs->texturesBinded[ii]) //dont need to worry about the locations of these textures in the vbs yet
                    {
                        texturesFound++;
                        textureBindsIds.push_back(to);
                        break;
                    }
                }
            }

            std::vector<textureObject*> gameObjectTexturesCopy = gameObject->to;
            if(vbs->texturesBinded.size() + gameObject->to.size() - textureBindsIds.size() <= 8)//if theres room to store the aditional textures if needed
            {
                for (size_t ii = 0; ii < gameObjectTexturesCopy.size(); ii++)
                {
                    for(textureObject* textureBinds : textureBindsIds)
                    {
                        if(textureBinds == gameObjectTexturesCopy[ii])
                        {
                            gameObjectTexturesCopy.erase(gameObjectTexturesCopy.begin() + ii); 
                            ii=0;
                        }
                    }
                }
                //gameobectTexturesCopy now contains the textures that need to be added to the vbs todo under lines and func

                for(textureObject* to : gameObjectTexturesCopy)
                {
                    vbs->texturesBinded.push_back(to);
                }

                updateTextureBinding(vbs, gameObject);
                vbs->bindedGameObjects.push_back(gameObject);
                return;
            }
        }

        //no texture already binded, can bind new check
        for(vertexBufferStruct* vbs : possiblevbs)
        {
            if(vbs->texturesBinded.size() + gameObject->to.size() < 8) //todo set to device max texture bind later.
            {
                //bind textures
                for(textureObject* to : gameObject->to)
                {
                    vbs->texturesBinded.push_back(to);
                }
                updateTextureBinding(vbs, gameObject);

                vbs->bindedGameObjects.push_back(gameObject);
                return;
            }
        }
    }

    //seems like no suitable vertex buffer was found, create a new one

    vertexBufferStruct* vbs = new vertexBufferStruct;
    vertexBuffers.push_back(vbs);
    vbs->viIndex = viIndex1;


    vbs->bindedGameObjects.push_back(gameObject);

    //bind shaders
    for(textureObject* to : gameObject->to)
    {
        vbs->texturesBinded.push_back(to);
    }
    updateTextureBinding(vbs, gameObject);

    if(batch)
    {
        vbs->maxObjects = std::max((maxVertPerVertexBuffer/gameObject->viData->verticies), (unsigned int)1);

        //vertex buffer object
        glGenBuffers(1, &vbs->vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vbs->vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, vbs->maxObjects * gameObject->viData->verticies, nullptr, GL_DYNAMIC_DRAW);


        //index buffer object - autogen for future use

        const Uint32 indexBufferSize = gameObject->viData->indicies;
        const Uint32 allIndexindexBufferSizes = vbs->maxObjects * indexBufferSize;

        vbs->fullIndexDataSize = allIndexindexBufferSizes;
        vbs->indexData = (Uint32*)malloc(allIndexindexBufferSizes);
        vbs->fullNumberOfElements = vbs->fullIndexDataSize/sizeof(Uint32);
        //vbs->shader = gameObject-> fix shader per object
        vbs->orthoProj = gameObject->orthoProj;

        vbs->fullVertexDataSize = vbs->maxObjects * gameObject->viData->verticies;
        vbs->eachVertexSize = gameObject->viData->verticies;
        vbs->vertexData = (float*)malloc(vbs->fullVertexDataSize);

        Uint32* srcData = (Uint32*)malloc(indexBufferSize);
        memcpy((void*)srcData, gameObject->viData->indexData, indexBufferSize);

        const Uint32* dstData = vbs->indexData;

        for (size_t ii = 0; ii < vbs->maxObjects; ii++)
        {
            memcpy((void*)((char*)dstData + (ii * indexBufferSize)), srcData, indexBufferSize);//remove char* conversion
            
            //increment the index data  
            for(size_t iii = 0; iii < indexBufferSize/sizeof(unsigned int); iii++)
            {
                srcData[iii] += (vbs->eachVertexSize/sizeof(float))/9;
            }
        }
        
        free(srcData);
    }
    else
    {
        vbs->noBatch = true;
        //vertex buffer object
        glGenBuffers(1, &vbs->vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vbs->vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, gameObject->viData->verticies, nullptr, GL_DYNAMIC_DRAW);
        //index buffer object - autogen for future use

        vbs->fullIndexDataSize = gameObject->viData->indicies;
        vbs->indexData = (Uint32*)malloc(vbs->fullIndexDataSize);
        vbs->fullNumberOfElements = vbs->fullIndexDataSize/sizeof(Uint32);
        //vbs->shader = gameObject-> fix shader per object
        vbs->orthoProj = gameObject->orthoProj;

        vbs->eachVertexSize = gameObject->viData->verticies;
        vbs->fullVertexDataSize = vbs->eachVertexSize;
        vbs->vertexData = (float*)malloc(vbs->fullVertexDataSize);

        memcpy((void*)vbs->indexData, gameObject->viData->indexData, vbs->fullIndexDataSize);
    }

    glGenBuffers(1, &vbs->indexbuffer);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbs->indexbuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vbs->fullIndexDataSize, vbs->indexData, GL_STATIC_DRAW);
}

static inline void updateTextureBinding(vertexBufferStruct* vbs, gameToRenderObject* gameObject) //texture index of the vbs - todo make changing textures not in vbs
{
    
    
}

static inline glm::vec3 yawPitchDirectionCalc(float yaw, float pitch)
{
    const glm::vec3 direction (cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)),sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    return direction;
}