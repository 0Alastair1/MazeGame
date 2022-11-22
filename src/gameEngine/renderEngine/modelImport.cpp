static inline void importModel(const std::string &name, const char *filePathC)
{
    std::string filePath = filePathC;

    for (rawModelStruct *rawModel : rawModels)
    {
        if (rawModel->name == name)
        {
            return; // maybe or override?
        }
    }

    // convert path inputted into platform specific
    char* slash = getSlash(); //memory issues when getSlash returned char?
    std::replace(filePath.begin(), filePath.end(), '/', slash[0]);
    std::replace(filePath.begin(), filePath.end(), '\\', slash[0]);
    free(slash);

    printf("%s", filePath.c_str());
}

static inline rawModelStruct *getModel(std::string name)
{
    for (rawModelStruct *rawModel : rawModels)
    {
        if (rawModel->name == name)
        {
            return rawModel;
        }
    }

    return nullptr;
}

static inline char* getSlash()
{
    // calc the flashes
    boost::filesystem::path currentDirPath = boost::dll::program_location().parent_path();
    std::string currentDir = currentDirPath.string();

    for (int i = currentDir.size() - 1; i >= 0; i--)
    {

        if (currentDir[i] == '\\' || currentDir[i] == '/')
        {
            char* character = (char*)malloc(sizeof(char));
            memcpy((void*)character, &currentDir[i], sizeof(char));
            return character;
        }
    }

    return nullptr; //?
}