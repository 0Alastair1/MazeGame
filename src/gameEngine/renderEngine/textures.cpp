static inline void genTextures() //textures are made automaticlly when added into the textures folder
{
    /*
    boost::filesystem::path currentDirPath = boost::dll::program_location().parent_path();
    const char* currentDir = currentDirPath.string().c_str();
    const char* texturesFolder = "/textures/";
    char* textureDirectory = (char*)malloc(strlen(currentDir) + strlen(texturesFolder));
    strcpy(textureDirectory,(char*)currentDir);
    strcat(textureDirectory,(char*)texturesFolder);

    std::vector<const char*> textureNames;

    for (const auto & entry : std::filesystem::directory_iterator(textureDirectory))
    {
        if(std::filesystem::is_directory(entry.path()))
        {
            continue;
        }

        textureNames.push_back(std::filesystem::path{entry.path()}.filename().string().c_str());
    }
    */
   //textureNames.push_back("wood.png");

    stbi_set_flip_vertically_on_load(1);

    //for(const char* textureName : textureNames)
    //{
        
        //char* filePath = (char*)malloc(strlen(textureDirectory) + strlen(textureName));
        //strcpy(filePath,(char*)textureDirectory);
        //strcat(filePath,(char*)textureName);
        //free(textureDirectory);
        const char* filePath = "/home/computer/Documents/GitHub/MazeGame/build/textures/wood.png";

        int width = 0;
        int height = 0;
        int bpp = 0;
        unsigned char* buffer = stbi_load(filePath, &width, &height, &bpp, 4);
        //free(filePath);

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
        texture->textureName = "wood.png";
        textureObjects.push_back(texture);
   // }
}

static inline textureObject* getTexture(const char* textureName)
{
    for(textureObject* texture : textureObjects)
    {
        if(!strcmp(texture->textureName.c_str(), textureName))
        {
            return texture;
        }
    }
    printf("texture not found");
    return textureObjects[0];
}