static inline void importModel(const std::string &name, const char *filePathC, const bool flip)
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

    assimpToModel(name, filePath, flip);
}


static inline void assimpToModel(const std::string &name, std::string filePath, const bool flip)
{
    rawModelStruct *rawModel = new rawModelStruct;
    rawModel->name = name;

    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

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

                if (currentMesh->HasTangentsAndBitangents())
                {
                    while(k >= rawModelData->tangents.size())
                    {
                        rawModelData->tangents.push_back(glm::vec3());
                        rawModelData->bitangents.push_back(glm::vec3());
                    }
                    
                    rawModelData->tangents[k] = (glm::vec3(currentMesh->mTangents[k].x, currentMesh->mTangents[k].y, currentMesh->mTangents[k].z));
                    rawModelData->bitangents[k] = (glm::vec3(currentMesh->mBitangents[k].x, currentMesh->mBitangents[k].y, currentMesh->mBitangents[k].z));
                }


                for(size_t l = 0; l < 1; l++)
                {
                    if (currentMesh->HasTextureCoords(l))
                    {
                        rawModelData->texCoords.push_back(std::vector<glm::vec2>());
                        rawModelData->texCoords[l].push_back(glm::vec2());

                        while(l > rawModelData->texCoords.size()) rawModelData->texCoords.push_back(std::vector<glm::vec2>());

                        while(k > rawModelData->texCoords[l].size()) rawModelData->texCoords[l].push_back(glm::vec2());

                            rawModelData->texCoords[l][k] = (glm::vec2(currentMesh->mTextureCoords[l][k].x, currentMesh->mTextureCoords[l][k].y));
                    }
                    else
                    {
                        break;
                    }
                }
               // else if (currentMesh->HasVertexColors())
                //{
                    //
                //}
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

                std::string texturePath = getCurrentDirectory() + slashS + filePath;
                std::string filePathParent = texturePath.substr(0, texturePath.find_last_of("/\\"));

                makeTexture(filePathParent + slashS + strs, str.C_Str(), diffuse, flip);
                rawModelData->textureNames.push_back(str.C_Str());
            }
            for(size_t k = 0; k < currentMaterial->GetTextureCount(aiTextureType_SPECULAR); k++)
            {
                aiString str;
		        currentMaterial->GetTexture(aiTextureType_SPECULAR, k, &str);
                const std::string strs = str.C_Str();

                char *slash = getSlash();
                std::string slashS;
                slashS.assign(slash, 1);
                free(slash);

                std::string texturePath = getCurrentDirectory() + slashS + filePath;
                std::string filePathParent = texturePath.substr(0, texturePath.find_last_of("/\\"));

                makeTexture(filePathParent + slashS + strs, str.C_Str(), specular, flip);
                rawModelData->textureNames.push_back(str.C_Str());
            }
            for(size_t k = 0; k < currentMaterial->GetTextureCount(aiTextureType_HEIGHT); k++)
            {
                aiString str;
		        currentMaterial->GetTexture(aiTextureType_HEIGHT, k, &str);
                const std::string strs = str.C_Str();

                char *slash = getSlash();
                std::string slashS;
                slashS.assign(slash, 1);
                free(slash);

                std::string texturePath = getCurrentDirectory() + slashS + filePath;
                std::string filePathParent = texturePath.substr(0, texturePath.find_last_of("/\\"));

                makeTexture(filePathParent + slashS + strs, str.C_Str(), height, flip);
                rawModelData->textureNames.push_back(str.C_Str());
            }
            for(size_t k = 0; k < currentMaterial->GetTextureCount(aiTextureType_AMBIENT); k++)
            {
                aiString str;
		        currentMaterial->GetTexture(aiTextureType_AMBIENT, k, &str);
                const std::string strs = str.C_Str();

                char *slash = getSlash();
                std::string slashS;
                slashS.assign(slash, 1);
                free(slash);

                std::string texturePath = getCurrentDirectory() + slashS + filePath;
                std::string filePathParent = texturePath.substr(0, texturePath.find_last_of("/\\"));

                makeTexture(filePathParent + slashS + strs, str.C_Str(), ambient, flip);
                rawModelData->textureNames.push_back(str.C_Str());
            }
            for(size_t k = 0; k < currentMaterial->GetTextureCount(aiTextureType_NORMALS); k++)
            {
                aiString str;
		        currentMaterial->GetTexture(aiTextureType_NORMALS, k, &str);
                const std::string strs = str.C_Str();

                char *slash = getSlash();
                std::string slashS;
                slashS.assign(slash, 1);
                free(slash);

                std::string texturePath = getCurrentDirectory() + slashS + filePath;
                std::string filePathParent = texturePath.substr(0, texturePath.find_last_of("/\\"));

                makeTexture(filePathParent + slashS + strs, str.C_Str(), normal, flip);
                rawModelData->textureNames.push_back(str.C_Str());
            }
            for(size_t k = 0; k < currentMaterial->GetTextureCount(aiTextureType_LIGHTMAP); k++)
            {
                aiString str;
		        currentMaterial->GetTexture(aiTextureType_LIGHTMAP, k, &str);
                const std::string strs = str.C_Str();

                char *slash = getSlash();
                std::string slashS;
                slashS.assign(slash, 1);
                free(slash);

                std::string texturePath = getCurrentDirectory() + slashS + filePath;
                std::string filePathParent = texturePath.substr(0, texturePath.find_last_of("/\\"));

                makeTexture(filePathParent + slashS + strs, str.C_Str(), light, flip);
                rawModelData->textureNames.push_back(str.C_Str());
            }
            for(size_t k = 0; k < currentMaterial->GetTextureCount(aiTextureType_EMISSIVE); k++)
            {
                aiString str;
		        currentMaterial->GetTexture(aiTextureType_EMISSIVE, k, &str);
                const std::string strs = str.C_Str();

                char *slash = getSlash();
                std::string slashS;
                slashS.assign(slash, 1);
                free(slash);

                std::string texturePath = getCurrentDirectory() + slashS + filePath;
                std::string filePathParent = texturePath.substr(0, texturePath.find_last_of("/\\"));

                makeTexture(filePathParent + slashS + strs, str.C_Str(), emissive, flip);
                rawModelData->textureNames.push_back(str.C_Str());
            }
            for(size_t k = 0; k < currentMaterial->GetTextureCount(aiTextureType_SHININESS); k++)
            {
                aiString str;
		        currentMaterial->GetTexture(aiTextureType_SHININESS, k, &str);
                const std::string strs = str.C_Str();

                char *slash = getSlash();
                std::string slashS;
                slashS.assign(slash, 1);
                free(slash);

                std::string texturePath = getCurrentDirectory() + slashS + filePath;
                std::string filePathParent = texturePath.substr(0, texturePath.find_last_of("/\\"));

                makeTexture(filePathParent + slashS + strs, str.C_Str(), shininess, flip);
                rawModelData->textureNames.push_back(str.C_Str());
            }
            for(size_t k = 0; k < currentMaterial->GetTextureCount(aiTextureType_CLEARCOAT); k++)
            {
                aiString str;
		        currentMaterial->GetTexture(aiTextureType_CLEARCOAT, k, &str);
                const std::string strs = str.C_Str();

                char *slash = getSlash();
                std::string slashS;
                slashS.assign(slash, 1);
                free(slash);

                std::string texturePath = getCurrentDirectory() + slashS + filePath;
                std::string filePathParent = texturePath.substr(0, texturePath.find_last_of("/\\"));

                makeTexture(filePathParent + slashS + strs, str.C_Str(), clearcoat, flip);
                rawModelData->textureNames.push_back(str.C_Str());
            }
            for(size_t k = 0; k < currentMaterial->GetTextureCount(aiTextureType_METALNESS); k++)
            {
                aiString str;
		        currentMaterial->GetTexture(aiTextureType_METALNESS, k, &str);
                const std::string strs = str.C_Str();

                char *slash = getSlash();
                std::string slashS;
                slashS.assign(slash, 1);
                free(slash);

                std::string texturePath = getCurrentDirectory() + slashS + filePath;
                std::string filePathParent = texturePath.substr(0, texturePath.find_last_of("/\\"));

                makeTexture(filePathParent + slashS + strs, str.C_Str(), metalness, flip);
                rawModelData->textureNames.push_back(str.C_Str());
            }

            for(size_t k = 0; k < currentMaterial->GetTextureCount(aiTextureType_LIGHTMAP); k++)
            {
                aiString str;
		        currentMaterial->GetTexture(aiTextureType_LIGHTMAP, k, &str);
                const std::string strs = str.C_Str();

                char *slash = getSlash();
                std::string slashS;
                slashS.assign(slash, 1);
                free(slash);

                std::string texturePath = getCurrentDirectory() + slashS + filePath;
                std::string filePathParent = texturePath.substr(0, texturePath.find_last_of("/\\"));

                makeTexture(filePathParent + slashS + strs, str.C_Str(), light, flip);
                rawModelData->textureNames.push_back(str.C_Str());
            }

            {
                aiColor3D col(0.f, 0.f, 0.f);

                currentMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, col);
                rawModelData->diffuse = glm::vec3(col.r, col.b, col.g);

                currentMaterial->Get(AI_MATKEY_COLOR_AMBIENT, col);
                rawModelData->ambient = glm::vec3(col.r, col.b, col.g);

                currentMaterial->Get(AI_MATKEY_COLOR_SPECULAR, col);
                rawModelData->specular = glm::vec3(col.r, col.b, col.g);

                currentMaterial->Get(AI_MATKEY_SHININESS, rawModelData->shininess);
            }
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