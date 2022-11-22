static inline void importModel(const std::string& name, const char* filePathC)
{
    std::string filePath = filePathC;

    for(rawModelStruct* rawModel : rawModels)
    {
        if(rawModel->name == name)
        {
            return; //maybe or override?
        }
    }

    //convert path inputted into platform specific
    char slash = getSlash();
    std::replace( filePath.begin(), filePath.end(), '/', slash);
    std::replace( filePath.begin(), filePath.end(), '\\', slash);

    printf("%s", filePath);
}



static inline rawModelStruct* getModel(std::string name)
{
    for(rawModelStruct* rawModel : rawModels)
    {
        if(rawModel->name == name)
        {
            return rawModel;
        }
    }

    return nullptr;
}

static inline char getSlash()
{
    //calc the flashes
    boost::filesystem::path currentDirPath = boost::dll::program_location().parent_path();
    std::string currentDir = currentDirPath.string();

    for (int i = currentDir.size() - 1; i >= 0; i--) {

        if(currentDir[i] == '\\' || currentDir[i] == '/')
        {
            return currentDir[i];
        }
    }

    return '/'; //?
}