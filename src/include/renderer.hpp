static inline void initRender();

struct textureObject
{
    const char* textureName;
    unsigned int textureID;
};
static std::vector<textureObject*> textureObjects;
struct shaderObject
{
    unsigned int shaderID;
    const char* shaderName;
};
static std::vector<shaderObject*> shaderObjects;
struct renderObject
{
    Uint8 id; //id for whatever use
    Uint32 indicies; //number of indices to be drawn from the index buffer
    const char* name; //name in string of the object
    const shaderObject* shader; //contains the shaders
    unsigned int vao; //contains vertex and index buffer
    const textureObject* texture;
};
static std::vector<renderObject*> renderObjects;

struct gameToRenderObject
{
    Uint8 id; //corrasponds to the render object to use
    glm::vec4 position;
    glm::vec4 scale;
    glm::vec4 rotation;
};
static std::vector<gameToRenderObject*> gameToRenderObjects;


static inline void initRenderObjects();
static inline void setup_default_shaders();
static inline void make_render_object_type(const Uint8 id, const float* vertexData, const unsigned int vertexDataSize, const unsigned int numberofCollums, const unsigned int* indices, const unsigned int indicesSize, const char* name, const char* shaderName, const char* textureName);
static inline void genTextures();
static inline textureObject* getTexture(const char* textureName);
static inline shaderObject* getShader(const char* shaderName);
static inline void performUniformOperation(const shaderObject* program);
static inline void makeShader(const char* vertexSrc, const char* fragmentSrc, const char* shaderName);