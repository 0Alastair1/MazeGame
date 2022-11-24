

enum textureTypeEnum
{
    diffuse,
    normal,
    specular,
    height,
};

struct textureObject
{
    std::string textureName;
    Uint32 textureID;
};

static std::vector<textureObject*> textureObjects;
static inline void makeTexture(std::string filePath, std::string textureName, textureTypeEnum textureType);
static inline std::string getTextureDirectory();
static inline textureObject* getTexture(std::string textureName);
static inline std::string getCurrentDirectory();