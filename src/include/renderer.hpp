static inline void initRender();

struct textureObject
{
    std::string textureName;
    unsigned int textureID;
};
static std::vector<textureObject*> textureObjects;
struct shaderObject
{
    unsigned int shaderID;
    std::string shaderName;
    Sint32 textureUniformLocations[8];
    Sint32 u_colorUniformLocation;
    Sint32 u_mvpUniformLocation;
    std::unordered_map<const char*, Sint32> nonGenericUniformlocations;
};
static std::vector<shaderObject*> shaderObjects;
struct renderObject
{
    Uint8 id; //id for whatever use
    Uint32 indicies; //number of indices to be drawn from the index buffer
    std::string name; //name in string of the object
    const shaderObject* shader; //contains the shaders
    unsigned int vao; //contains vertex and index buffer
    std::vector<const textureObject*> textures;
};
static std::vector<renderObject*> renderObjects;

static inline renderObject* getRenderObject(const char* renderObjectName);

struct gameToRenderObject
{
    Uint32 id;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec4 rotation;
    renderObject* renderObj; //render object to use
    bool orthoProj;

    gameToRenderObject()
    {
        id = 0;
        position = {0.0f, 0.0f, 0.0f};
        scale = { 1.0f, 1.0f, 1.0f};
        rotation = {0.0f ,0.0f, 0.0f, 0.0f};
        renderObj = getRenderObject("square");
        orthoProj = false;
    }
};
static std::vector<gameToRenderObject*> gameToRenderObjects;

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

static inline void initRenderObjects();
static inline void setup_default_shaders();
static inline void make_render_object_type(const Uint8 id, const float* vertexData, const unsigned int vertexDataSize, const unsigned int numberofCollums, const unsigned int* indices, const unsigned int indicesSize, const char* name, const char* shaderName, std::vector<const char*> textureNames);
static inline void genTextures();
static inline textureObject* getTexture(const char* textureName);
static inline shaderObject* getShader(const char* shaderName);
static inline void performUniformOperation(const gameToRenderObject* gameObject, const glm::mat4 viewMatrix, const glm::mat4 perspectiveProjectionMatrix);
static inline void makeShader(const char* vertexSrc, const char* fragmentSrc, const char* shaderName);
static inline gameToRenderObject* makeGameObject();
static inline void deleteGameObject(gameToRenderObject* gameObject);