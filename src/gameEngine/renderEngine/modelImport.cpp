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
    std::string slash = getSlash(); //memory issues when getSlash returned char?
    std::replace(filePath.begin(), filePath.end(), '/', slash.c_str()[0]);
    std::replace(filePath.begin(), filePath.end(), '\\', slash.c_str()[0]);

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

static inline std::string getSlash()
{
    // calc the flashes
    boost::filesystem::path currentDirPath = boost::dll::program_location().parent_path();
    std::string currentDir = currentDirPath.string();

    for (int i = currentDir.size() - 1; i >= 0; i--)
    {

        if (currentDir[i] == '\\' || currentDir[i] == '/')
        {
            return  currentDir.substr(0, 1);;
        }
    }

    return "/"; //?
}