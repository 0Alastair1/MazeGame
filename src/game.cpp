
static inline void initGame()
{
    for(size_t i = 0; i < 1'000'0; i++)
    {
        const float triangleData[] = {
        -0.5f, -0.5f, 0.0f,/*unused*/ 1.0f, 0.0f, 0.0f, /*tex cords*/ -1.0f, -1.0f, /*texture index, automatically set*/ 0.0f,
        0.5f, -0.5f, 0.0f, /*unused*/ 0.0f, 1.0f, 0.0f, /*tex cords*/  1.0f, -1.0f, /*texture index, automatically set*/ 0.0f,
        0.5f,  0.5f, 0.0f, /*unused*/ 0.0f, 0.0f, 1.0f, /*tex cords*/  1.0f, 1.0f,  /*texture index, automatically set*/ 0.0f,
        -0.5f,  0.5f, 0.0f,/*unused*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ -1.0f, 1.0f,  /*texture index, automatically set*/ 0.0f
        };
        const unsigned int numberofCollums = 8;
        const unsigned int triangleIndecies[] = {
            0, 1, 2,
            0, 2, 3
        };
        gameToRenderObject* gameObject = makeGameObject(&triangleData[0], &triangleIndecies[0], sizeof(triangleData), sizeof(triangleIndecies), "wood.png", false);
        glClearColor(0.0f, 0.0f, 0.0, 0.0); //bug somewhere, remove this

        mainCamera.position.x = -20.0f;
        
        const float amount = 1000;
        float v2 = (rand() % (int)amount + 1)/3; 
        float v3 = (rand() % (int)amount + 1)/3; 
        float v4 = (rand() % (int)amount + 1)/3; 
        gameObject->changePos(v2-amount/4, v3-amount/4, v4-amount/2+(1000/2));
        //gameObject->changeRotationGlobal(0.0, 0.0, 0.0);
    }

    fpsMouse(true);
}


static inline void gameLoop()
{
    const float speed = 0.01f;
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
    for(gameToRenderObject* gameObject : gameToRenderObjects)
    {

        //gameObject->changePos(gameObject->position.x, gameObject->position.y, gameObject->position.z);
        gameObject->changeRotationGlobal(1.0f, 0.0f,0.0f);
        //gameObject->lookAt(mainCamera.position);

        i+= 1.0f;
    }
}