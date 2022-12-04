static inline void initRender();
static inline void render();


static inline textureObject* getTexture(const char* textureName);
struct shaderObject
{
    Sint32 shaderID;
    std::string shaderName;
    Sint32 textureUniformLocations[32];
    Sint32 u_colorUniformLocation;
    Sint32 u_mvpUniformLocation;
    Sint32 u_ambientLightColLocation;
    Sint32 u_ambientLightIntensityLocation;
    Sint32 u_matAmbientLightColLocation;
    std::unordered_map<const char*, Sint32> nonGenericUniformlocations; //remove maybe
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

    glm::vec4* colorMul;

    verticesindexesData* viData; //data of verticies
    std::vector<textureObject*> to; //binded textureobject

    //textures
    Sint32 diffuse;
    Sint32 normal;
    Sint32 specular;
    Sint32 height;
    Sint32 ambient; 
    Sint32 light;
    Sint32 emissive;
    Sint32 shininess;
    Sint32 clearcoat;
    Sint32 metalness;

    glm::vec3 ambientLightCol;

    gameToRenderObject(float* cobjectData,
        unsigned int* cindexData,
        Uint32 cverticies,
        Uint32 cindicies,
        const std::vector<std::string>& textureNames,
        bool orthoProject,
        const glm::vec3 cambientColor)
    {
        this->id = 0;
        this->position = glm::vec3(0.0f, 0.0f, 0.0f);
        this->scale = { 1.0f, 1.0f, 1.0f};
        this->rotation = {0.0f ,0.0f, 0.0f};
        this->orthoProj = orthoProject;
        this->ambientLightCol = cambientColor;

        this->viData = new verticesindexesData;

        this->viData->verticies = cverticies;
        this->viData->indicies = cindicies;
        this->viData->objectData = (const float*)malloc(this->viData->verticies);
        this->viData->indexData = (const unsigned int*)malloc(this->viData->indicies);
        memcpy((void*)this->viData->objectData, cobjectData, this->viData->verticies);
        memcpy((void*)this->viData->indexData, cindexData, this->viData->indicies);
        //free(cobjectData);
        //free(cindexData);

        for(size_t i = 0; i < textureNames.size(); i++)
        {
            textureObject* to2 = getTexture(textureNames[i]); 
            to.push_back(to2);
            const textureObject* to1 = to2; 

            switch(to1->type)
            {
                case textureTypeEnum::diffuse:
                    this->diffuse = to1->textureID;
                    break;
                case textureTypeEnum::normal:
                    this->normal = to1->textureID;
                    break;
                case textureTypeEnum::specular:
                    this->specular = to1->textureID;
                    break;
                case textureTypeEnum::height:
                    this->height = to1->textureID;
                    break;
                case textureTypeEnum::ambient:
                    this->ambient = to1->textureID;
                    break;
                case textureTypeEnum::light:
                    this->light = to1->textureID;
                    break;
                case textureTypeEnum::emissive:
                    this->emissive = to1->textureID;
                    break;
                case textureTypeEnum::shininess:
                    this->shininess = to1->textureID;
                    break;
                case textureTypeEnum::clearcoat:
                    this->clearcoat = to1->textureID;
                    break;
                case textureTypeEnum::metalness:
                    this->metalness = to1->textureID;
                    break;
                                    
                default:
                    break;
            }
        }

        verticiesChanged = true;
    }
    void update(glm::mat4 modelProj, bool left)
    {
        //apply given matrix to the models verticies that need transformation

        for(size_t i =0; i < (this->viData->verticies/sizeof(float))/numberOfCollums; i++)
        {
            glm::vec4 tmpVertex; //= glm::vec4(glm::make_vec3(&this->viData->objectData[i * numberOfCollums]), 1.0f);
            tmpVertex.x = this->viData->objectData[i * numberOfCollums];
            tmpVertex.y = this->viData->objectData[i * numberOfCollums + 1];
            tmpVertex.z = this->viData->objectData[i * numberOfCollums + 2];
            tmpVertex.w = 1.0f;

            if(left)
            {
                tmpVertex = modelProj * tmpVertex;
            }
            else
            {
                tmpVertex = tmpVertex * modelProj;
            }

            memcpy((void*)&this->viData->objectData[i * numberOfCollums], (glm::value_ptr(tmpVertex)), sizeof(float) * 3);

            this->verticiesChanged = true;
        }
    }
    void changePos(float x, float y, float z)
    {
        glm::vec3 toPosVector = this->position - glm::vec3(x, y, z);
        this->position = glm::vec3(x, y, z);
        glm::mat4 modelPosition = glm::translate(glm::mat4(1.0f), toPosVector);
        this->update(modelPosition, true);
    }
    /*
    void changeScale(float x, float y, float z) //todo
    {
        this->scale = glm::vec3(x, y, z );
        //this->update();
    }
    */
    void changeRotation(float x, float y, float z)
    {
        auto rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::normalize(glm::vec3(1,0,0)));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), glm::radians(y), glm::normalize(glm::vec3(0,1,0)));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::normalize(glm::vec3(0,0,1)));

        glm::mat4 modelPosition = glm::translate(glm::mat4(1.0f), this->position);

        this->update(rotationMatrix * modelPosition, true);
    }
    void changeRotationGlobal(float x, float y, float z)
    {
        this->rotation = glm::vec3(x,y,z);//? maybe make a get rotatoin func?
        auto rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::normalize(glm::vec3(1,0,0)));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), glm::radians(y), glm::normalize(glm::vec3(0,1,0)));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), glm::radians(x), glm::normalize(glm::vec3(0,0,1)));
        this->update(rotationMatrix, true);
    }
    void lookAt(glm::vec3 loc) //broken
    {
        static glm::mat4 prevMatrix = glm::mat4();
        const glm::mat4 lookAtMatrix = glm::inverse(glm::lookAt(this->position, this->position + loc, glm::vec3(0, 1, 0)));
        this->update(lookAtMatrix - prevMatrix, true);
        prevMatrix = lookAtMatrix - prevMatrix;
    }

};

static inline void assignGameObjectToVertexBuffer(gameToRenderObject* gameObjectv, bool batch);
static std::vector<gameToRenderObject*> gameToRenderObjects;
struct vertexBufferStruct
{
    Sint32 viIndex; //index into uniqueViData used by renderer to extract the index buffer template
    std::vector<textureObject*> texturesBinded;
    std::vector<gameToRenderObject*> bindedGameObjects;

    shaderObject* shader;
    bool orthoProj;

    //opengl vertex buffer and index buffer
    unsigned int vertexbuffer;//change this to Uint32
    unsigned int indexbuffer;

    //copy of the data to send to opengl
    Uint32* indexData;
    Uint32 fullIndexDataSize;
    Uint32 fullNumberOfElements;

    float* vertexData;
    Uint32 fullVertexDataSize;

    Uint32 eachVertexSize;
    Uint32 eachIndexBufferSize;

    Uint32 maxObjects;
    bool noBatch;
    glm::vec4 colorMul = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
static inline gameToRenderObject* makeGameObject(float* cobjectData, unsigned int* cindexData, Uint32 cverticies, Uint32 cindicies, const std::vector<std::string>& textureNames, bool orthoProject, bool batch, const glm::vec3 ambientCol);
static inline glm::vec3 yawPitchDirectionCalc(float yaw, float pitch);
static inline void uniFormPerFrame();
static inline gameToRenderObject* makeGameObject(std::vector<glm::vec3>& cobjectData, std::vector<Uint32>& cindexData, Uint32 cverticies, Uint32 cindicies, const std::vector<std::string>& textureNames, bool orthoProject, bool batch, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<glm::vec3>& tangents, const glm::vec3 ambientCol);
static inline void updateIndexArray(vertexBufferStruct* vbs);

static float color = 0.0f;
static float colorIncrease = 0.001f;