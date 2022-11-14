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

struct gameToRenderObject
{
    Uint32 id;
    glm::vec4 position;
    glm::vec4 scale;
    glm::vec4 rotation;
    renderObject* renderObj; //render object to use
};
static std::vector<gameToRenderObject*> gameToRenderObjects;

static inline void initRenderObjects();
static inline void setup_default_shaders();
static inline void make_render_object_type(const Uint8 id, const float* vertexData, const unsigned int vertexDataSize, const unsigned int numberofCollums, const unsigned int* indices, const unsigned int indicesSize, const char* name, const char* shaderName, std::vector<const char*> textureNames);
static inline void genTextures();
static inline textureObject* getTexture(const char* textureName);
static inline shaderObject* getShader(const char* shaderName);
static inline void performUniformOperation(const gameToRenderObject* gameObject);
static inline void makeShader(const char* vertexSrc, const char* fragmentSrc, const char* shaderName);
static inline renderObject* getRenderObject(const char* renderObjectName);
static inline gameToRenderObject* makeGameObject();
static inline void deleteGameObject(gameToRenderObject* gameObject);