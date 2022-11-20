static inline void initRender();
static inline void render();

struct textureObject
{
    std::string textureName;
    Uint32 textureID;
};
static std::vector<textureObject*> textureObjects;

static inline textureObject* getTexture(const char* textureName);
struct shaderObject
{
    Sint32 shaderID;
    std::string shaderName;
    Sint32 textureUniformLocations[32];
    Sint32 u_colorUniformLocation;
    Sint32 u_mvpUniformLocation;
    std::unordered_map<const char*, Sint32> nonGenericUniformlocations;
};
static std::vector<shaderObject*> shaderObjects;

struct verticesindexesData
{
    const float* objectData;//used for two purposes. once when created to get original vertex data and next for object data
    const unsigned int* indexData; //used when drawing
    Uint32 verticies;
    Uint32 indicies; 
};

static std::vector<verticesindexesData*> uniqueViData;

static inline textureObject* getTexture(const char* textureName);
static inline shaderObject* getShader(const char* shaderName);

struct gameToRenderObject
{

    Uint32 id;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    //only update the objectdata if previous stuff changed
    glm::vec3 previousPosition;
    glm::vec3 previousScale;
    glm::vec4 previousRotation;
    bool verticiesChanged;
    
    bool orthoProj;

    verticesindexesData* viData; //data of verticies
    textureObject* to; //binded textureobject
    Uint8 bindedTextureSlot;

    gameToRenderObject(const float* cobjectData,
        const unsigned int* cindexData,
        Uint32 cverticies,
        Uint32 cindicies,
        const char* textureName,
        bool orthoProject)
    {
        this->id = 0;
        this->position = glm::vec3(0.0f, 0.0f, 0.0f);
        this->scale = { 1.0f, 1.0f, 1.0f};
        this->rotation = {0.0f ,0.0f, 0.0f};
        this->orthoProj = orthoProject;

        this->viData = new verticesindexesData;

        this->viData->verticies = cverticies;
        this->viData->indicies = cindicies;
        this->viData->objectData = (const float*)malloc(this->viData->verticies);
        this->viData->indexData = (const unsigned int*)malloc(this->viData->indicies);
        memcpy((void*)this->viData->objectData, cobjectData, this->viData->verticies);
        memcpy((void*)this->viData->indexData, cindexData, this->viData->indicies);

        to = getTexture(textureName);

        verticiesChanged = true;
    }
    void update(glm::mat4 modelProj, bool left)
    {
        //apply given matrix to the models verticies

        for(size_t i =0; i < ((this->viData->verticies/4)/3)/3; i++)
        {
            glm::vec4 tmpChange = glm::vec4(glm::make_vec3(&this->viData->objectData[i * 9]), 1.0f);

            if(left)
                tmpChange = modelProj * tmpChange;
            else
                tmpChange = tmpChange * modelProj;

            memcpy((void*)&this->viData->objectData[i * 9], (glm::value_ptr(tmpChange)), sizeof(float) * 3);
        }
    }
    void changePos(float x, float y, float z)
    {
        glm::vec3 toPosVector = this->position - glm::vec3(x, y, z);
        this->position = glm::vec3(x, y, z);
        glm::mat4 modelPosition = glm::translate(glm::mat4(1.0f), toPosVector);
        this->update(modelPosition, true);
        this->verticiesChanged = true;
    }
    
    void movePosBy(float x, float y, float z)
    {
        this->position = glm::vec3(x, y, z);
        glm::mat4 modelPosition = glm::translate(glm::mat4(1.0f), this->position);
        this->update(modelPosition, true);
        this->verticiesChanged = true;
    }
    /*
    void changeScale(float x, float y, float z) //todo
    {
        this->scale = glm::vec3(x, y, z );
        //this->update();
        this->verticiesChanged = true;
    }
    */
   /*
    void changeRotation(float x, float y, float z)
    {
        auto rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::normalize(glm::vec3(1,0,0)));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), glm::radians(y), glm::normalize(glm::vec3(0,1,0)));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::normalize(glm::vec3(0,0,1)));
        this->update(rotationMatrix, false);

        this->verticiesChanged = true;
    }*/
    void changeRotationGlobal(float x, float y, float z)
    {
        auto rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::normalize(glm::vec3(1,0,0)));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), glm::radians(y), glm::normalize(glm::vec3(0,1,0)));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::normalize(glm::vec3(0,0,1)));
        this->update(rotationMatrix, true);

        this->verticiesChanged = true;
    }
    void lookAt(glm::vec3 loc) //broken
    {
        const glm::mat4 lookAtMatrix = glm::inverse(glm::lookAt(this->position, loc, glm::vec3(0, 1, 0)));
        this->update(lookAtMatrix, true);
    }

};

static inline void assignGameObjectToVertexBuffer(gameToRenderObject* gameObjectv, bool batch);
static std::vector<gameToRenderObject*> gameToRenderObjects;
struct vertexBufferStruct
{
    Sint32 viIndex; //index into uniqueViData used by renderer to extract the index buffer template
    std::vector<textureObject*> texturesBinded;
    std::vector<gameToRenderObject*> bindedGameObjects;

    shaderObject* shader = getShader("defaultShader");
    bool orthoProj;

    //opengl vertex buffer and index buffer
    unsigned int vertexbuffer;//change this to Uint32
    unsigned int indexbuffer;

    //copy of the data to send to opengl
    Uint32* indexData;
    Uint32 fullIndexDataSize;
    Uint32 fullNumberOfElements;

    const float* vertexData;
    Uint32 fullVertexDataSize;

    Uint32 eachVertexSize;
};
static std::vector<vertexBufferStruct*> vertexBuffers;

struct camera
{
    glm::vec3 position;
    glm::vec4 rotation;
    glm::mat4 orthoProjectionMatrix;
    glm::mat4 perspectiveProjectionMatrix;

    float pitch;
    float yaw;
    float roll;//dont include in vector calculations

    glm::vec3 cameraFacingDirVec = glm::vec3(0.0f, 0.0f, -1.0f);
    const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

    camera()
    {
        position = glm::vec3(0.0f, 0.0f, 3.0f);
        yaw = -90.0f;
    }
} static mainCamera;

/*
struct gpuStruct
{
    Uint32 vramSize;
    Uint32 availableTextureSlots;


} const gpu;*/

static inline void setup_default_shaders();
static inline void genTextures();
static inline void makeShader(const char* vertexSrc, const char* fragmentSrc, const char* shaderName);
static inline gameToRenderObject* makeGameObject(const float* cobjectData, const unsigned int* cindexData, Uint32 cverticies, Uint32 cindicies, const char* textureName, bool orthoProject, bool batch);
static inline glm::vec3 yawPitchDirectionCalc(float yaw, float pitch);
static inline void updateTextureBinding(Uint8 textureIndex, gameToRenderObject* gameObject);
static inline void uniFormPerFrame();

static float color = 0.0;
static float colorIncrease = 0.001;