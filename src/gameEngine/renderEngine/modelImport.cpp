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
    char *slash = getSlash(); // memory issues when getSlash returned char?
    std::replace(filePath.begin(), filePath.end(), '/', slash[0]);
    std::replace(filePath.begin(), filePath.end(), '\\', slash[0]);
    free(slash);

    printf("%s", filePath.c_str());

    assimpToModel(name, filePath);
}


static inline void assimpToModel(const std::string &name, std::string filePath)
{
    rawModelStruct *rawModel = new rawModelStruct;
    rawModel->name = name;

    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        return;
    }
    std::string directory = filePath.substr(0, filePath.find_last_of('/'));

    std::vector<aiNode *> nodes;

    nodes.push_back(scene->mRootNode);

    for (size_t i = 0; i < nodes.size(); i++)
    {

        for (size_t j = 0; j < nodes[i]->mNumChildren; j++)
        {
            nodes.push_back(nodes[i]->mChildren[j]);
        }

        if(!(nodes[i]->mNumMeshes > 0))
        {
            continue;
        }

        for (size_t j = 0; j < nodes[i]->mNumMeshes; j++)
        {
            const aiMesh* currentMesh = scene->mMeshes[nodes[i]->mMeshes[j]];
            rawModelDataStruct *rawModelData = new rawModelDataStruct;
            rawModelData->name = currentMesh->mName.data;
            rawModelData->numVertices = currentMesh->mNumVertices;
            rawModelData->numIndices = 0;
            rawModel->includedModels.push_back(rawModelData);

            for (unsigned int k = 0; k < currentMesh->mNumVertices; k++)
            {

                rawModelData->positions.push_back(glm::vec3((float)currentMesh->mVertices[k].x, (float)currentMesh->mVertices[k].y, (float)currentMesh->mVertices[k].z));


                if (currentMesh->HasNormals())
                {
                    rawModelData->normals.push_back(glm::vec3((float)currentMesh->mNormals[k].x, (float)currentMesh->mNormals[k].y, (float)currentMesh->mNormals[k].z));
                }

                for(size_t l = 0; l < 8; l++)
                {
                    if (currentMesh->mTextureCoords[l] != nullptr)
                    {
                        rawModelData->texCoords.push_back(std::vector<glm::vec2>());
                        rawModelData->tangents.push_back(std::vector<glm::vec3>());
                        rawModelData->bitangents.push_back(std::vector<glm::vec3>());

                        rawModelData->texCoords[l].push_back(glm::vec2(currentMesh->mTextureCoords[l]->x, currentMesh->mTextureCoords[l]->y));
                        //rawModelData->tangents[l].push_back(glm::vec3(currentMesh->mTangents[l].x, currentMesh->mTangents[l].y, currentMesh->mTangents[l].z));
                        //rawModelData->bitangents[l].push_back(glm::vec3(currentMesh->mBitangents[l].x, currentMesh->mBitangents[l].y, currentMesh->mBitangents[l].z));
                    }
                    else
                    {
                        break;
                    }
                }
            }

            for (Uint32 k = 0; k < currentMesh->mNumFaces; k++)
            {
                for (Uint32 l = 0; l < currentMesh->mFaces[k].mNumIndices; l++)
                {
                    rawModelData->indices.push_back(currentMesh->mFaces[k].mIndices[l]);
                    rawModelData->numIndices++;
                }
            }

            aiMaterial* currentMaterial = scene->mMaterials[currentMesh->mMaterialIndex];
            
            for(size_t k = 0; k < currentMaterial->GetTextureCount(aiTextureType_DIFFUSE); k++)
            {
                aiString str;
		        currentMaterial->GetTexture(aiTextureType_DIFFUSE, k, &str);
                const std::string strs = str.C_Str();

                char *slash = getSlash();
                std::string slashS;
                slashS.assign(slash, 1);
                free(slash);

                makeTexture(getCurrentDirectory() + slashS + filePath + slashS + std::string("..") + slashS + strs, str.C_Str(), diffuse);
                rawModelData->textureNames.push_back(str.C_Str());
            }
            //aiTextureType_SPECULAR aiTextureType_HEIGHT aiTextureType_AMBIENT
        }
    }

    rawModels.push_back(rawModel);
}

static inline rawModelStruct* getModel(std::string name)
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

static inline char *getSlash()

{
    // calc the flashes
    boost::filesystem::path currentDirPath = boost::dll::program_location().parent_path();
    std::string currentDir = currentDirPath.string();

    for (int i = currentDir.size() - 1; i >= 0; i--)
    {

        if (currentDir[i] == '\\' || currentDir[i] == '/')
        {
            char *character = (char *)malloc(sizeof(char));
            memcpy((void *)character, &currentDir[i], sizeof(char));
            return character;
        }
    }

    return nullptr; //?
}