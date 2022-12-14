static float is = 0;

static inline void initGame()
{
    importModel("test", "../vendor/glTF-Sample-Models/2.0/Sponza/glTF/Sponza.gltf", false); //todo can be combineded into a single vertexbuffer use the meshes index and their textures to calc the texture id
    importModel("test", "../vendor/glTF-Sample-Models/2.0/aa/backpack.obj", true);
    rawModelStruct* modelRaw = getModel("test");
    if(modelRaw == nullptr)
    {
        importModel("test1", "../../vendor/glTF-Sample-Models/2.0/Sponza/glTF/Sponza.gltf", false);
        modelRaw = getModel("test1");
    }

    for(size_t i = 0; i < modelRaw->includedModels.size(); i++)
    {
        rawModelDataStruct* meshData = modelRaw->includedModels[i];
        gameToRenderObject* gameObject = makeGameObject(meshData->positions,meshData->indices, 
        meshData->numVertices * sizeof(glm::vec3), meshData->numIndices * sizeof(Uint32),meshData->textureNames, false, true, 
        meshData->normals, meshData->texCoords[0], meshData->tangents, meshData->ambient, meshData->diffuse);
    }

    importModel("test2", "../vendor/glTF-Sample-Models/2.0/SheenChair/glTF/SheenChair.gltf", false); //todo can be combineded into a single vertexbuffer use the meshes index and their textures to calc the texture id
    modelRaw = getModel("test2");
    if(modelRaw == nullptr)
    {
        importModel("test3", "../../vendor/glTF-Sample-Models/2.0/SheenChair/glTF/SheenChair.gltf", false);
        modelRaw = getModel("test3");
    }

    for(size_t i = 0; i < modelRaw->includedModels.size(); i++)
    {
        rawModelDataStruct* meshData = modelRaw->includedModels[i];
        gameToRenderObject* gameObject = makeGameObject(meshData->positions,meshData->indices, 
        meshData->numVertices * sizeof(glm::vec3), meshData->numIndices * sizeof(Uint32),meshData->textureNames, false, true, 
        meshData->normals, meshData->texCoords[0], meshData->tangents, meshData->ambient, meshData->diffuse);
    }

    
    //mainCamera.position = *(glm::vec3*)&(gameToRenderObjects[0]->viData->objectData[0]);

    makeTexture(getTextureDirectory() + "wood.png", "wood.png", diffuse, true);
    assignShaderToTexture("wood.png", "testShader");
    std::vector<std::string> textureNames = {"wood.png"};
    
    for(size_t i = 0; i < 1; i++)
    {
        float triangleData[] = {
            //front
            /*cords*/-0.5f, -0.5f, 0.0f,/*normals*/ 1.0f, 0.0f, 0.0f, /*tex cords*/ -1.0f, -1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/ 0.5f, -0.5f, 0.0f,/*normals*/ 0.0f, 1.0f, 0.0f, /*tex cords*/  1.0f, -1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/ 0.5f,  0.5f, 0.0f,/*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/  1.0f, 1.0f,  /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/-0.5f,  0.5f, 0.0f,/*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ -1.0f, 1.0f,  /*tangents*/ 0.0f, 0.0f, 0.0f,

            //left
            /*cords*/-0.5f, -0.5f, -1.0f, /*normals*/ 0.0f, 0.0f,1.0f, /*tex coord*/ -1.0f, 1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/-0.5f, -0.5f, 0.0f,/*normals*/ 1.0f, 0.0f, 0.0f, /*tex cords*/ 1.0f, -1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/-0.5f,  0.5f, 0.0f,/*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ 1.0f, 1.0f,  /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/-0.5f, 0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 0.0f, /*tex coord*/ -1.0f, -1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,


            //bottom
            /*cords*/-0.5f, -0.5f, -1.0f, /*normals*/ 0.0f, 0.0f,1.0f, /*tex coord*/ 1.0f, -1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/ 0.5f, -0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex coord*/ -1.0f, -1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/ 0.5f, -0.5f, 0.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/  -1.0f, 1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/-0.5f, -0.5f, 0.0f,/*normals*/ 1.0f, 0.0f, 0.0f, /*tex cords*/ 1.0f, 1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,


            //right
            /*cords*/ 0.5f, -0.5f, 0.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex coord*/ -1.0f, -1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/ 0.5f, -0.5f, -1.0f, /*normals*/ 1.0f, 0.0f, 0.0f, /*tex coord*/ 1.0f, -1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/ 0.5f,  0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ 1.0f, 1.0f,  /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/ 0.5f,  0.5f, 0.0f,/*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ -1.0f, 1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,

            //top
            /*cords*/-0.5f, 0.5f, 0.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex coord*/ -1.0f, -1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/ 0.5f, 0.5f, 0.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ 1.0f, -1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/ 0.5f, 0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ 1.0f, 1.0f,  /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/-0.5f, 0.5f, -1.0, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex coord*/ -1.0f, 1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,

            //back
            /*cords*/0.5, -0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex coord*/ -1.0f, -1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/ -0.5, -0.5, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex coord*/ 1.0f, -1.0f, /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/ -0.5, 0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ 1.0f, 1.0f,  /*tangents*/ 0.0f, 0.0f, 0.0f,
            /*cords*/ 0.5, 0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ -1.0f, 1.0f,  /*tangents*/ 0.0f, 0.0f, 0.0f,

        };
        unsigned int triangleIndecies[] = {
            0, 1, 2,
            0, 2, 3,

            4, 5, 6,
            6, 7, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,
            
            20, 21, 22,
            22, 23, 20
        };
        std::vector<std::string> textureNamesc = {"wood.png"};
        gameToRenderObject* gameObject = makeGameObject(&triangleData[0], &triangleIndecies[0], sizeof(triangleData), sizeof(triangleIndecies), textureNamesc, false, true, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        testObj = gameObject;
        
        //const float amount = 1000;
        //float v2 = (rand() % (int)amount + 1)/3; 
        //float v3 = (rand() % (int)amount + 1)/3; 
        //float v4 = (rand() % (int)amount + 1)/3; 
       // gameObject->changePos(v2-amount/4, v3-amount/4, v4-amount/2+(1000/2));
       //gameObject->lookAt(mainCamera.position);
        //gameObject->lookAt(mainCamera.position);
        //gameObject->changeRotationGlobal(0.0, 0.0, 0.0);
    }
    
    mainCamera.position = glm::vec3(0.0f, 0.0f,-25.0f);
    testObj->changePos(0.0f, 0.0f,-25.0f);
    
    for(gameToRenderObject* gameObject : gameToRenderObjects)
    {
        //gameObject->changePos(gameObject->position.x +(9000.0f), gameObject->position.y, gameObject->position.z);
    }
    //fpsMouse(true);
}


static inline void gameLoop()
{
    const float speed = 1.01f;
    const glm::vec3 cameraFacingDirVecC = mainCamera.cameraFacingDirVec;
    const glm::vec3 cameraUpC = mainCamera.cameraUp;

    static int i = 0;

    if(getKey('a') || getKey('A'))
    {
        mainCamera.position -= glm::normalize(glm::cross(cameraFacingDirVecC, cameraUpC)) * speed * deltaTime;
    }
    if(getKey('d') || getKey('D'))
    {
       mainCamera.position += glm::normalize(glm::cross(cameraFacingDirVecC, cameraUpC)) * speed * deltaTime;
    }
    if(getKey('s') || getKey('S'))
    {
        mainCamera.position -= speed * cameraFacingDirVecC * deltaTime;
    }
    if(getKey('w') || getKey('W'))
    {
       mainCamera.position += speed * cameraFacingDirVecC * deltaTime;
    }
    if(getKey(ESC) || getKey(HT))
    {
        fpsMouse(false);
        i++;
    }
    if(getMouseKey(false))
    {
        fpsMouse(true);
    }

   // i = (i % 400) - 400;
   
    for(gameToRenderObject* gameObject : gameToRenderObjects)
    {
        if(gameObject == testObj)
            break;
        //gameObject->changePos(gameObject->position.x +(20.0f), gameObject->position.y, gameObject->position.z);
        ///float random = (float)((rand() % 150)/ 100)/10*(float)deltaTime;
        //gameObject->changeRotationGlobal(random, random,random);
        //gameObject->lookAt(mainCamera.position);
       //gameObject->changeRotation(0.0f, 0.0f, 0.0f);
        //i+= 1.0f;
    }
}