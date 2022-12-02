

enum textureTypeEnum
{
    diffuse,
    normal,
    specular,
    height,
    ambient,
    light,
    emissive,
    shininess,
    clearcoat,
    metalness,
};

struct textureObject
{
    std::string textureName;
    Uint32 textureID;
    textureTypeEnum type;
};


static std::vector<textureObject*> textureObjects;

std::vector<std::string> textureToShader;
std::vector<std::string> shadersFromTexture;

static inline void assignShaderToTexture(const std::string& textureName, const std::string& shaderName);
static inline void makeTexture(std::string filePath, std::string textureName, textureTypeEnum textureType, const bool flip);
static inline std::string getTextureDirectory();
static inline textureObject* getTexture(std::string textureName);
static inline std::string getCurrentDirectory();

