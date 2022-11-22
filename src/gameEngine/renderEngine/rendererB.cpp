static Sint32 prevShader = -1;
static Uint32 texturesBindedprev[32];
static const Uint32 numObjectsVerVertex = 10000;

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
            if(texturesBindedprev[i] != vbs->texturesBinded[i]->textureID)
            {            
                glActiveTexture(GL_TEXTURE0 + i); 
                glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                glUniform1i(i, vbs->texturesBinded[i]->textureID);
                texturesBindedprev[i] = vbs->texturesBinded[i]->textureID;
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

    for(float i = 0; i < deltaTime/3; i+=1.0f) //remove
    {
        if(color > 0.60) colorIncrease = -0.001;
        else if(color < 0.40) colorIncrease = 0.001;
    
        color += colorIncrease;
    }

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
        glUniform4f(vbs->shader->u_colorUniformLocation, color+color/4, color-color/2, color+color/2, 1.0f);
    }
}

static inline void assignGameObjectToVertexBuffer(gameToRenderObject* gameObject, const bool batch)
{
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
            if(vbs->viIndex == viIndex1 && vbs->bindedGameObjects.size() < numObjectsVerVertex) //replace numObjectsVerVertex with vram size minus a bit? how to get vram size?
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

    //vertex buffer object
    glGenBuffers(1, &vbs->vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vbs->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, numObjectsVerVertex * gameObject->viData->verticies, nullptr, GL_DYNAMIC_DRAW);


    //index buffer object - autogen for future use

    const Uint32 indexBufferSize = gameObject->viData->indicies;
    const Uint32 allIndexindexBufferSizes = numObjectsVerVertex * indexBufferSize;

    vbs->fullIndexDataSize = allIndexindexBufferSizes;
    vbs->indexData = (Uint32*)malloc(allIndexindexBufferSizes);
    vbs->fullNumberOfElements = vbs->fullIndexDataSize/sizeof(Uint32);
    //vbs->shader = gameObject-> fix shader per object
    vbs->orthoProj = gameObject->orthoProj;

    vbs->fullVertexDataSize = numObjectsVerVertex * gameObject->viData->verticies;
    vbs->eachVertexSize = gameObject->viData->verticies;
    vbs->vertexData = (float*)malloc(vbs->fullVertexDataSize);

    Uint32* srcData = (Uint32*)malloc(indexBufferSize);
    memcpy((void*)srcData, gameObject->viData->indexData, indexBufferSize);

    const Uint32* dstData = vbs->indexData;

    for (size_t ii = 0; ii < numObjectsVerVertex; ii++)
    {
        memcpy((void*)((char*)dstData + (ii * indexBufferSize)), srcData, indexBufferSize);//remove char* conversion
        
        //increment the index data  
        for(size_t iii = 0; iii < indexBufferSize/sizeof(unsigned int); iii++)
        {
            srcData[iii] += (vbs->eachVertexSize/sizeof(float))/9;
        }
    }
    
    free(srcData);

    glGenBuffers(1, &vbs->indexbuffer);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbs->indexbuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, allIndexindexBufferSizes, dstData, GL_STATIC_DRAW);
}

static inline void updateTextureBinding(vertexBufferStruct* vbs, gameToRenderObject* gameObject) //texture index of the vbs - todo make changing textures not in vbs
{

    for(size_t i =0; i < ((gameObject->viData->verticies/4)/3)/3; i++) //todo for the other vertex's texture ids
    {
        float texureIndexFloat = -1.0f; //assumes textures for the object are actually in the vbs

        //get the texture name from the objects vertex id
        const float vertexIndex = gameObject->viData->objectData[(i* 9) + 8];
        const std::string thisTextureBinded = gameObject->to[(int)vertexIndex]->textureName;
        
        //convert that id to the corrasponding texture in the vbs and write back
        for(size_t ii = 0; i < vbs->texturesBinded.size(); i++)
        {
            if(thisTextureBinded == vbs->texturesBinded[ii]->textureName)
            {
                texureIndexFloat = i;
                break;
            }
        }

        //rewrite the float with the updated index
        memcpy((void*)&gameObject->viData->objectData[(i * 9) + 8], &texureIndexFloat, sizeof(float));
    }
}

static inline glm::vec3 yawPitchDirectionCalc(float yaw, float pitch)
{
    const glm::vec3 direction (cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)),sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    return direction;
}