

struct rawModelDataStruct
{
    std::string name;

    std::vector<glm::vec3> positions;
    std::vector<Uint32> indices;
    std::vector<glm::vec3> normals;
    std::vector<std::vector<glm::vec2>> texCoords;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float shininess = 1.0f;
    

    Uint32 numVertices;
    Uint32 numIndices;

    std::vector<std::string> textureNames;
};


struct rawModelStruct
{
    std::string name;
    std::vector<rawModelDataStruct*> includedModels;
};
std::vector<rawModelStruct*> rawModels;

static inline void importModel(const std::string& name, const char* filePathC, const bool flip);
static inline rawModelStruct* getModel(std::string name);
static inline char* getSlash();
static inline void assimpToModel(const std::string &name, std::string filePath, const bool flip);