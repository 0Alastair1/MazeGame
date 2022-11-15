static inline void initGame()
{
    gameToRenderObject* gameObject = makeGameObject();
    gameObject->id = 0;
    gameObject->position = {0.0f, 0.0f, 0.0f};
    gameObject->renderObj = getRenderObject("square");
    gameObject->rotation;
    gameObject->scale;
    gameObject->orthoProj = true;

    //mainCamera.position.x += -1.0f;
}


static inline void gameLoop()
{

}