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

        //performUniformOperation(vbs, viewMatrix);

        size_t gameObjectIndex = 0;
        const float* vertexDest =  vbs->vertexData;
        const Uint32 eachVertexSize = vbs->eachVertexSize;
        
        for(gameToRenderObject* gameObject : vbs->bindedGameObjects)
        {
            performUniformOperation(vbs, viewMatrix, gameObject);

            if(gameObject->verticiesChanged == false)
                continue;
            
            gameObject->verticiesChanged = false;
            memcpy((void*)(vertexDest + (gameObjectIndex * (eachVertexSize/sizeof(float)))), gameObject->viData->objectData, eachVertexSize);

            gameObjectIndex++;
        }

        //bind textures - todo optimize this

        for(size_t i = 0; i < vbs->texturesBinded.size(); i++)
        {
            switch(vbs->texturesBinded[i]->type) //to do multiple textures of same type
            {


                case textureTypeEnum::diffuse:
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[i], 0);
                    break;

                    case textureTypeEnum::normal:
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[i], 1);
                    break;

                case textureTypeEnum::specular:
                    glActiveTexture(GL_TEXTURE2);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[i], 2);
                    break;

                case textureTypeEnum::height:
                    glActiveTexture(GL_TEXTURE3);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[i], 3);
                    break;

                case textureTypeEnum::ambient:
                    glActiveTexture(GL_TEXTURE4);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[i], 4);
                    break;

                case textureTypeEnum::light:
                    glActiveTexture(GL_TEXTURE5);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[i], 5);
                    break;

                case textureTypeEnum::emissive:
                    glActiveTexture(GL_TEXTURE6);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[i], 6);
                    break;
                    
                case textureTypeEnum::shininess:
                    glActiveTexture(GL_TEXTURE7);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[i], 7);
                    break;

                case textureTypeEnum::clearcoat:
                    glActiveTexture(GL_TEXTURE8);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[i], 8);
                    break;

                case textureTypeEnum::metalness:
                    glActiveTexture(GL_TEXTURE9);
                    glBindTexture(GL_TEXTURE_2D, vbs->texturesBinded[i]->textureID);
                    glUniform1i(vbs->shader->textureUniformLocations[i], 9);
                    break;

                default:
                    break;
            }
        }

        //bind the buffers
        glBindBuffer(GL_ARRAY_BUFFER, vbs->vertexbuffer);
        //glBufferSubData(GL_ARRAY_BUFFER, 0, vbs->fullVertexDataSize, vertexDest);//suppose to be vbs->bindedGameObjects.size() * eachVertexSize??
        glBufferSubData(GL_ARRAY_BUFFER, 0, vbs->bindedGameObjects.size() * eachVertexSize, vertexDest);

        //the 0 corrasponds to the layout value in the shader
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, numberOfCollums*(sizeof(float)),(void*)0 );

       //color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, numberOfCollums*(sizeof(float)),(void*)( 3*sizeof(float) ));

        //texture coods
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, numberOfCollums*(sizeof(float)),(void*)( 6*sizeof(float) ));

        //tangents
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, numberOfCollums*(sizeof(float)),(void*)( 8*sizeof(float) ));//mm

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbs->indexbuffer);


        drawCalls++;
        glDrawElements(GL_TRIANGLES, vbs->fullNumberOfElements, GL_UNSIGNED_INT, nullptr);
    }

    render_imgui();

    SDL_GL_SwapWindow(window);
}

static inline void uniFormPerFrame() //?? use uniform buffer object
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

static inline void performUniformOperation(const vertexBufferStruct* vbs, const glm::mat4 viewMatrix, const gameToRenderObject* gameObject)//seperate into uniform object
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

    if(!strcmp(vbs->shader->shaderName.c_str(), "defaultShader")) //fixme dont hardcode in use forloop maybe not
    { 
        //set uniforms
        //glUniformMatrix3x4fv
        glUniformMatrix4fv(vbs->shader->u_mvpUniformLocation, 1, GL_FALSE, &vp[0][0]);
        glUniform3f(vbs->shader->u_ambientLightColLocation, generalLight.ambientLightCol.x, generalLight.ambientLightCol.y, generalLight.ambientLightCol.z);
        glUniform3f(vbs->shader->u_ambientLightIntensityLocation, generalLight.ambientLightIntensity.x, generalLight.ambientLightIntensity.y, generalLight.ambientLightIntensity.z);
        glUniform3f(vbs->shader->u_matAmbientLightColLocation, gameObject->ambientLightCol.x, gameObject->ambientLightCol.y, gameObject->ambientLightCol.z);
        //static float testin = 0.0f;
       // testin+= 0.0001f;
        //glUniform3f(vbs->shader->uDirectionLightLocLocation, generalLight.directionDiffuseDirection.x + testin, generalLight.directionDiffuseDirection.y, generalLight.directionDiffuseDirection.z);
       //glUniform3f(vbs->shader->uDirectionLightLocLocation, -4000.0f,0.0,0.0f);
       //glUniform3f(vbs->shader->uDirectionLightLocLocation, mainCamera.position.x, mainCamera.position.y, mainCamera.position.z);
       //printf("posx = %f, posy = %f, posz = %f", mainCamera.position.x, mainCamera.position.y, mainCamera.position.z);
      // printf("\n");
        glUniform3f(vbs->shader->uDirectionLightLocLocation, testObj->position.x, testObj->position.y, testObj->position.z);
        glUniform3f(vbs->shader->uDirectionLightIntensityLocation, generalLight.directionDiffuseIntensity.x, generalLight.directionDiffuseIntensity.y, generalLight.directionDiffuseIntensity.z);
        glUniform3f(vbs->shader->uDiffuseLightColLocation, gameObject->diffuseLightCol.x, gameObject->diffuseLightCol.y, gameObject->diffuseLightCol.z);
    }
    if(!strcmp(vbs->shader->shaderName.c_str(), "testShader")) //fixme dont hardcode in use forloop maybe not
    { 
        //set uniforms
        //glUniformMatrix3x4fv
        glUniformMatrix4fv(vbs->shader->u_mvpUniformLocation, 1, GL_FALSE, &vp[0][0]);
        glUniform4f(vbs->shader->u_colorUniformLocation, vbs->colorMul.x, vbs->colorMul.y, vbs->colorMul.z, 1.0f);
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
            if(vbs->viIndex == viIndex1 && vbs->bindedGameObjects.size() < vbs->maxObjects)
            {
                possiblevbs.push_back(vbs);
            }
        }

        //search for vbs with exact textures binded
        std::vector<vertexBufferStruct*> possiblevbs2; 
        for(vertexBufferStruct* vbs : possiblevbs)
        {
            std::vector<textureObject*>textureBindsIds;
            for(size_t ii = 0; ii < vbs->texturesBinded.size(); ii++)
            {
                for(textureObject* to : gameObject->to)
                {
                    if(to == vbs->texturesBinded[ii])
                    {
                        textureBindsIds.push_back(to);
                        break;
                    }
                }
            }

            if(gameObject->to.size() == textureBindsIds.size())
            {
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
    gameObject->colorMul = &vbs->colorMul;


    vbs->shader = getShader("defaultShader");

    //bind shaders
    for(textureObject* to : gameObject->to)
    {
        vbs->texturesBinded.push_back(to);
        for(size_t i = 0; i < textureToShader.size(); i++)
        {
            if(to->textureName == textureToShader[i])
            {
                vbs->shader = getShader(shadersFromTexture[i].c_str());
            }
        }
    }

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
                srcData[iii] += (vbs->eachVertexSize/sizeof(float))/numberOfCollums;
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vbs->fullIndexDataSize, vbs->indexData, GL_STATIC_DRAW);
}

static inline glm::vec3 yawPitchDirectionCalc(float yaw, float pitch)
{
    const glm::vec3 direction (cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)),sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    return direction;
}