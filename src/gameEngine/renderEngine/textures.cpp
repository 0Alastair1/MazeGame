static inline void genTextures() //textures are made automaticlly when added into the textures folder
{

}

static inline void makeTexture(std::string filePath, std::string textureName, textureTypeEnum textureType, const bool flip)
{
    for(textureObject* to : textureObjects)
    {
        if(to->textureName == textureName)
        {
            return;
        }
    }

    int width = 0;
    int height = 0;
    int bpp = 0;

    stbi_set_flip_vertically_on_load(flip);
    auto buffer = stbi_load(filePath.c_str(), &width, &height, &bpp, 4);
    printf("%s - %s\n", stbi_failure_reason(), filePath.c_str());

    unsigned int textureID = 0;

    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    switch(textureType)
    {
        case textureTypeEnum::diffuse:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            break;
        case textureTypeEnum::normal:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            break;

        default:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            break;
    }
    
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    textureObject* texture = new textureObject;
    texture->textureID = textureID;
    texture->type = textureType;
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
    std::string currentDir = getCurrentDirectory();

    char* slashes = getSlash();

    std::string texturesFolder = slashes[0] + (std::string)"textures" + slashes[0];
    free(slashes);
    std::string textureDirectory = currentDir + texturesFolder;

    std::filesystem::create_directory(textureDirectory);

    return textureDirectory;
}

static inline std::string getCurrentDirectory()
{
    char* currentDirChar = SDL_GetBasePath();
    std::string currentDir = currentDirChar;
    //free(currentDirChar); //todo check if commenting this out causes memory leak

    return currentDir;
}