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
    gameToRenderObject* gameObject = new gameToRenderObject(&triangleData[0], &triangleIndecies[0], sizeof(triangleData), sizeof(triangleIndecies), "wood.png", true);
    assignGameObjectToVertexBuffer(gameObject);
    gameToRenderObjects.push_back(gameObject);


    gameObject->update();

    //mainCamera.position.x += -1.0f;
}


static inline void gameLoop()
{

}