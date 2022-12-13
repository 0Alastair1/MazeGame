
static inline void performUniformOperation(const vertexBufferStruct* vbs, const glm::mat4 viewMatrix, const gameToRenderObject* gameObject);

struct generalLightStruct {
    glm::vec3 ambientLightCol = glm::vec3{1.0f, 1.0f, 1.0f};
    glm::vec3 ambientLightIntensity = glm::vec3{0.15f, 0.15f, 0.15f};

    glm::vec3 directionDiffuseDirection = glm::vec3{0.0f, 0.0f, 0.0f};
    glm::vec3 directionDiffuseIntensity = glm::vec3{10.0f, 10.0f, 10.0f};
} static generalLight;