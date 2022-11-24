static float is = 0;
static inline void initGame()
{
    importModel("test", "../vendor/glTF-Sample-Models/2.0/Sponza/glTF/Sponza.gltf", false); //todo can be combineded into a single vertexbuffer use the meshes index and their textures to calc the texture id
    //importModel("test", "../vendor/glTF-Sample-Models/2.0/aa/backpack.obj", true);
    rawModelStruct* modelRaw = getModel("test");

    for(size_t i = 0; i < modelRaw->includedModels.size(); i++)
    {
        rawModelDataStruct* meshData = modelRaw->includedModels[i];
        gameToRenderObject* gameObject = makeGameObject(meshData->positions,meshData->indices, 
        meshData->numVertices * sizeof(glm::vec3), meshData->numIndices * sizeof(Uint32),meshData->textureNames, false, true, false, 
        meshData->normals, meshData->texCoords[0] );
    }

    mainCamera.position = *(glm::vec3*)&(gameToRenderObjects[0]->viData->objectData[0]);

    makeTexture(getTextureDirectory() + "wood.png", "wood.png", diffuse, true);
    std::vector<std::string> textureNames = {"wood.png"};
    
    for(size_t i = 0; i < 1; i++)
    {
        float triangleData[] = {
            //front
            /*cords*/-0.5f, -0.5f, 0.0f,/*normals*/ 1.0f, 0.0f, 0.0f, /*tex cords*/ -1.0f, -1.0f, /*texture id*/ 0.0f,
            /*cords*/ 0.5f, -0.5f, 0.0f,/*normals*/ 0.0f, 1.0f, 0.0f, /*tex cords*/  1.0f, -1.0f, /*texture id*/ 0.0f,
            /*cords*/ 0.5f,  0.5f, 0.0f,/*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/  1.0f, 1.0f,  /*texture id*/ 0.0f,
            /*cords*/-0.5f,  0.5f, 0.0f,/*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ -1.0f, 1.0f,  /*texture id*/ 0.0f,

            //left
            /*cords*/-0.5f, -0.5f, -1.0f, /*normals*/ 0.0f, 0.0f,1.0f, /*tex coord*/ -1.0f, 1.0f, /*texture id*/ 0.0f,
            /*cords*/-0.5f, -0.5f, 0.0f,/*normals*/ 1.0f, 0.0f, 0.0f, /*tex cords*/ 1.0f, -1.0f, /*texture id*/ 0.0f,
            /*cords*/-0.5f,  0.5f, 0.0f,/*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ 1.0f, 1.0f,  /*texture id*/ 0.0f,
            /*cords*/-0.5f, 0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 0.0f, /*tex coord*/ -1.0f, -1.0f, /*texture id*/ 0.0f,


            //bottom
            /*cords*/-0.5f, -0.5f, -1.0f, /*normals*/ 0.0f, 0.0f,1.0f, /*tex coord*/ 1.0f, -1.0f, /*texture id*/ 0.0f,
            /*cords*/ 0.5f, -0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex coord*/ -1.0f, -1.0f, /*texture id*/ 0.0f,
            /*cords*/ 0.5f, -0.5f, 0.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/  -1.0f, 1.0f, /*texture id*/ 0.0f,
            /*cords*/-0.5f, -0.5f, 0.0f,/*normals*/ 1.0f, 0.0f, 0.0f, /*tex cords*/ 1.0f, 1.0f, /*texture id*/ 0.0f,


            //right
            /*cords*/ 0.5f, -0.5f, 0.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex coord*/ -1.0f, -1.0f, /*texture id*/ 0.0f,
            /*cords*/ 0.5f, -0.5f, -1.0f, /*normals*/ 1.0f, 0.0f, 0.0f, /*tex coord*/ 1.0f, -1.0f, /*texture id*/ 0.0f,
            /*cords*/ 0.5f,  0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ 1.0f, 1.0f,  /*texture id*/ 0.0f,
            /*cords*/ 0.5f,  0.5f, 0.0f,/*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ -1.0f, 1.0f, /*texture id*/ 0.0f,

            //top
            /*cords*/-0.5f, 0.5f, 0.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex coord*/ -1.0f, -1.0f, /*texture id*/ 0.0f,
            /*cords*/ 0.5f, 0.5f, 0.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ 1.0f, -1.0f, /*texture id*/ 0.0f,
            /*cords*/ 0.5f, 0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ 1.0f, 1.0f,  /*texture id*/ 0.0f,
            /*cords*/-0.5f, 0.5f, -1.0, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex coord*/ -1.0f, 1.0f, /*texture id*/ 0.0f,

            //back
            /*cords*/0.5, -0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex coord*/ -1.0f, -1.0f, /*texture id*/ 0.0f,
            /*cords*/ -0.5, -0.5, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex coord*/ 1.0f, -1.0f, /*texture id*/ 0.0f,
            /*cords*/ -0.5, 0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ 1.0f, 1.0f,  /*texture id*/ 0.0f,
            /*cords*/ 0.5, 0.5f, -1.0f, /*normals*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ -1.0f, 1.0f,  /*texture id*/ 0.0f,

        };
        const unsigned int numberofCollums = 8;
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
        gameToRenderObject* gameObject = makeGameObject(&triangleData[0], &triangleIndecies[0], sizeof(triangleData), sizeof(triangleIndecies), textureNamesc, false, true, false);
        glClearColor(0.0f, 0.0f, 0.0, 0.0); //bug somewhere, remove this

        mainCamera.position.x = -20.0f;
        
        const float amount = 1000;
        float v2 = (rand() % (int)amount + 1)/3; 
        float v3 = (rand() % (int)amount + 1)/3; 
        float v4 = (rand() % (int)amount + 1)/3; 
        gameObject->changePos(v2-amount/4, v3-amount/4, v4-amount/2+(1000/2));
       //gameObject->lookAt(mainCamera.position);
        //gameObject->lookAt(mainCamera.position);
        //gameObject->changeRotationGlobal(0.0, 0.0, 0.0);
    }


    //fpsMouse(true);
}


static inline void gameLoop()
{
    const float speed = 0.1f;
    const glm::vec3 cameraFacingDirVecC = mainCamera.cameraFacingDirVec;
    const glm::vec3 cameraUpC = mainCamera.cameraUp;

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
    }
    if(getMouseKey(false))
    {
        fpsMouse(true);
    }

    float i = 0;
    //for(gameToRenderObject* gameObject : gameToRenderObjects)
    //{

        //gameObject->changePos(gameObject->position.x, gameObject->position.y, gameObject->position.z);
        ///float random = (float)((rand() % 150)/ 100)/10*(float)deltaTime;
        //gameObject->changeRotationGlobal(random, random,random);
        //gameObject->lookAt(mainCamera.position);

        //i+= 1.0f;
    //}
}