static inline void initRender();

struct textureObject
{
    std::string textureName;
    unsigned int textureID;
};
static std::vector<textureObject*> textureObjects;

static inline textureObject* getTexture(const char* textureName);
struct shaderObject
{
    unsigned int shaderID;
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
    glm::vec4 rotation;

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
        this->position = {0.0f, 0.0f, 0.0f};
        this->scale = { 1.0f, 1.0f, 1.0f};
        this->rotation = {0.0f ,0.0f, 0.0f, 0.0f};
        this->orthoProj = orthoProject;

        this->viData->verticies = cverticies;
        this->viData->indicies = cindicies;
        this->viData->objectData = (const float*)malloc(this->viData->verticies);
        this->viData->indexData = (const unsigned int*)malloc(this->viData->indicies);
        memcpy((void*)this->viData->objectData, cobjectData, this->viData->verticies);

        to = getTexture(textureName);

        verticiesChanged = true;
    }
    void update()
    {
        //updates the objects verticies by its position, scale and rotation
        //this->viData->verticies

        //model projection 
        glm::mat4 modelPosition = glm::translate(glm::mat4(1.0f), this->position);
        glm::mat4 modelRotation;
        glm::mat4 modelScale;

        glm::mat4 modelProj = modelPosition;

        for(size_t i =0; i < ((this->viData->verticies/4)/3)/3; i+=9)
        {
            glm::vec4 tmpChange = glm::vec4(glm::make_vec3(&this->viData->objectData[i]), 1.0f);

            tmpChange = modelProj * tmpChange;

            memcpy((void*)&this->viData->objectData[i], (glm::value_ptr(tmpChange)), sizeof(float) * 3);
        }
    }
};

static inline void assignGameObjectToVertexBuffer(gameToRenderObject* gameObjectv);
static std::vector<gameToRenderObject*> gameToRenderObjects;
struct vertexBufferStruct
{
    Uint32 viIndex; //index into uniqueViData used by renderer to extract the index buffer template
    std::vector<textureObject*> texturesBinded;
    std::vector<gameToRenderObject*> bindedGameObjects;

    shaderObject* shader = getShader("defaultShader");
    bool orthoProj;

    //opengl vertex buffer and index buffer
    unsigned int vertexbuffer;
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
    camera()
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        rotation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
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
static inline gameToRenderObject* makeGameObject();