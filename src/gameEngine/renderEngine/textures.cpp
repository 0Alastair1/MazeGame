static inline void genTextures() //textures are made automaticlly when added into the textures folder
{
    /*
    boost::filesystem::path currentDirPath = boost::dll::program_location().parent_path();
    std::string currentDir = currentDirPath.string();

    char* slashes = getSlash();

    std::string texturesFolder = slashes[0] + (std::string)"textures" + slashes[0];
    free(slashes);
    std::string textureDirectory = currentDir + texturesFolder;

    if(!boost::filesystem::exists(textureDirectory))
    {
        boost::filesystem::create_directories(textureDirectory);
    }

    std::vector<std::string> textureNames;

    for (const auto & entry : std::filesystem::directory_iterator(textureDirectory))
    {
        if(std::filesystem::is_directory(entry.path()))
        {
            continue;
        }

        textureNames.push_back(std::filesystem::path{entry.path()}.filename().string());
    }

    stbi_set_flip_vertically_on_load(1);

    for(std::string textureName : textureNames)
    {
        
        std::string filePath = textureDirectory + textureName;

        makeTexture(filePath, textureName);
    }*/
}

static inline void makeTexture(std::string filePath, std::string textureName, textureTypeEnum textureType)
{
    int width = 0;
    int height = 0;
    int bpp = 0;
    unsigned char* buffer = stbi_load(filePath.c_str(), &width, &height, &bpp, 4);

    unsigned int textureID = 0;

    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    textureObject* texture = new textureObject;
    texture->textureID = textureID;
    texture->textureName = textureName;
    textureObjects.push_back(texture);
}

static inline textureObject* getTexture(std::string textureName)
{
    for(textureObject* texture : textureObjects)
    {
        if(!strcmp(texture->textureName.c_str(), textureName.c_str()))
        {
            return texture;
        }
    }
    printf("texture not found");
    return textureObjects[0];
}

static inline void test()
{}

static inline std::string getTextureDirectory()
{
    boost::filesystem::path currentDirPath = boost::dll::program_location().parent_path();
    std::string currentDir = currentDirPath.string();

    char* slashes = getSlash();

    std::string texturesFolder = slashes[0] + (std::string)"textures" + slashes[0];
    free(slashes);
    std::string textureDirectory = currentDir + texturesFolder;

    if(!boost::filesystem::exists(textureDirectory))
    {
        boost::filesystem::create_directories(textureDirectory);
    }

    return textureDirectory;
}

static inline std::string getCurrentDirectory()
{
    boost::filesystem::path currentDirPath = boost::dll::program_location().parent_path();
    std::string currentDir = currentDirPath.string();

    return currentDir;
}