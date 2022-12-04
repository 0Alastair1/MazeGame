
static inline void performUniformOperation(const vertexBufferStruct* vbs, const glm::mat4 viewMatrix, const gameToRenderObject* gameObject);

struct generalLightStruct {
    glm::vec3 ambientLightCol = glm::vec3{1.0f, 1.0f, 1.0f};
    glm::vec3 ambientLightIntensity = glm::vec3{1.0f, 1.0f, 1.0f};
} static generalLight;