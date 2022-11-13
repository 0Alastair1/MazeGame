static inline void initRender();
static inline unsigned int makeShader(const char* vertexSrc, const char* fragmentSrc);

struct renderObject
{
    Uint8 id; //id for whatever use
    Uint32 indicies; //number of indices to be drawn from the index buffer
    const char* name; //name in string of the object
    unsigned int program; //contains the shaders
    unsigned int vao; //contains vertex and index buffer
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

static inline void initRenderObjects(const unsigned int defaultShader);
static inline unsigned int setup_default_shaders();
static inline void make_render_object_type(const Uint8 id, const float* vertexData, const unsigned int vertexDataSize, const unsigned int vertexAmount, const unsigned int* indices, const unsigned int indicesSize, const char* name, const unsigned int shader);
static inline void genTextures();