static const char defaultVsSource[] =
{
    "#version 330 core \n \
    layout(location = 0) in vec3 vertexes; \
    layout(location = 1) in vec3 normals; \
    layout(location = 2) in vec2 vTexCord; \
    layout(location = 3) in vec3 tangents; \
    \
    uniform mat4 vp; \
    \
    out vec2 texCord; \
    out mat3 tbn; \
    \
    void main()	\
    {	\
        gl_Position = vp * vec4(vertexes, 1.0);	\
        texCord = vTexCord; \
        \
        vec3 tangent = normalize(vec3(vp * vec4(tangents, 0.0)));\
        vec3 normal = normalize(vec3(vp * vec4(normals, 0.0)));\
        tangent = normalize(tangent - dot(tangent, normal) * normal);\
        vec3 biTangent = cross(normal, tangent);\
        tbn = mat3(tangent, biTangent, normal);\
        \
    }"
};

static const char defaultFsSource[] =
{
    "#version 330 core \n    \
    layout(location = 0) out vec4 color;	\
    \
    in vec2 texCord; \
    in mat3 tbn; \
    \
    uniform vec3 ambientLightCol;\
    uniform vec3 ambientLightIntensity;\
    \
    uniform vec3 matAmbientLightCol;\
    \
    uniform sampler2D TextureSlot0; /*diffuse*/ \
    uniform sampler2D TextureSlot1; /*normal*/ \
    uniform sampler2D TextureSlot2; /*specular*/ \
    uniform sampler2D TextureSlot3; /*height*/ \
    uniform sampler2D TextureSlot4; /*ambient*/ \
    uniform sampler2D TextureSlot5; /*light*/ \
    uniform sampler2D TextureSlot6; /*emissive*/ \
    uniform sampler2D TextureSlot7; /*shininess*/ \
    uniform sampler2D TextureSlot8; /*clearcoat*/ \
    uniform sampler2D TextureSlot9; /*metalness*/ \
    \
    void main()	\
    {	\
        color = texture(TextureSlot0, texCord) * (vec4(ambientLightCol, 1.0f) * vec4(ambientLightIntensity, 1.0f)) * vec4(matAmbientLightCol, 1.0); \
    }"
};

//test

static const char testVsSource[] =
{
    "#version 330 core \n \
    layout(location = 0) in vec3 vertexes; \
    layout(location = 1) in vec3 normals; \
    layout(location = 2) in vec2 vTexCord; \
    layout(location = 3) in vec3 tangents; \
    \
    uniform vec4 u_Color; \
    uniform mat4 vp; \
    \
    out vec4 fColor;\
    out vec2 texCord; \
    out mat3 tbn; \
    \
    void main()	\
    {	\
        gl_Position = vp * vec4(vertexes, 1.0);	\
        fColor = u_Color; \
        texCord = vTexCord; \
        \
        vec3 tangent = normalize(vec3(vp * vec4(tangents, 0.0)));\
        vec3 normal = normalize(vec3(vp * vec4(normals, 0.0)));\
        tangent = normalize(tangent - dot(tangent, normal) * normal);\
        vec3 biTangent = cross(normal, tangent);\
        tbn = mat3(tangent, biTangent, normal);\
        \
    }"
};

static const char testFsSource[] =
{
    "#version 330 core \n    \
    layout(location = 0) out vec4 color;	\
    \
    /*uniform vec4 u_Color;*/ \
    \
    in vec4 fColor; \
    in vec2 texCord; \
    in mat3 tbn; \
    \
    void main()	\
    {	\
        color = fColor; \
    }"
};
 