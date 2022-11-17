
static inline void initGame()
{
    const float triangleData[] = {
    -0.5f, -0.5f, 0.0f,/*color*/ 1.0f, 0.0f, 0.0f, /*tex cords*/ -1.0f, -1.0f, /*texture index, automatically set*/ 0.0f,
    0.5f, -0.5f, 0.0f, /*color*/ 0.0f, 1.0f, 0.0f, /*tex cords*/  1.0f, -1.0f, /*texture index, automatically set*/ 0.0f,
    0.5f,  0.5f, 0.0f, /*color*/ 0.0f, 0.0f, 1.0f, /*tex cords*/  1.0f, 1.0f,  /*texture index, automatically set*/ 0.0f,
    -0.5f,  0.5f, 0.0f,/*color*/ 0.0f, 0.0f, 1.0f, /*tex cords*/ -1.0f, 1.0f,  /*texture index, automatically set*/ 0.0f
    };
    const unsigned int numberofCollums = 8;
    const unsigned int triangleIndecies[] = {
        0, 1, 2,
        0, 2, 3
    };
    gameToRenderObject* gameObject = makeGameObject(&triangleData[0], &triangleIndecies[0], sizeof(triangleData), sizeof(triangleIndecies), "wood.png", false);
    glClearColor(0.0f, 0.0f, 0.0, 0.0); //bug somewhere, remove this

    gameObject->update();

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
}