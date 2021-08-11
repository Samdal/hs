#define HS_DEFAULT_TRIANGLE {                   \
                -0.5f, -0.5f, 0.0f,             \
                0.5f, -0.5f, 0.0f,              \
                0.0f,  0.5f, 0.0f}

#define HS_DEFAULT_SQUARE_VERT_ONLY {           \
                0.5f,  0.5f, 0.0f,              \
                0.5f, -0.5f, 0.0f,              \
                -0.5f,  0.5f, 0.0f,             \
                0.5f, -0.5f, 0.0f,              \
                -0.5f, -0.5f, 0.0f,             \
                -0.5f,  0.5f, 0.0f}

#define HS_DEFAULT_SQUARE_VERTECIES {           \
                0.5f,  0.5f, 0.0f,              \
                0.5f, -0.5f, 0.0f,              \
                -0.5f, -0.5f, 0.0f,             \
                -0.5f,  0.5f, 0.0f }

#define HS_DEFAULT_SQUARE_SCALED(scale) {         \
                scale,  scale, 0.0f,              \
                scale, -scale, 0.0f,              \
                -scale, -scale, 0.0f,             \
                -scale,  scale, 0.0f }
#define HS_DEFAULT_SQUARE_INDECIES {0, 1, 3, 1, 2, 3}

#define HS_DEFAULT_SQUARE_TEX_VERT_ONLY {        \
                0.5f,  0.5f, 0.0f, 1.0f, 0.0f,   \
                0.5f, -0.5f, 0.0f, 1.0f, 1.0f,   \
                -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  \
                0.5f, -0.5f, 0.0f, 1.0f, 1.0f,   \
                -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  \
                -0.5f,  0.5f, 0.0f, 0.0f, 0.0f}

#define HS_DEFAULT_SQUARE_SCALED_TEX_VERT_ONLY(width, height) {     \
                width,  height, 1.0f, 0.0f,          \
                width, -height, 1.0f, 1.0f,          \
                -width,  height, 0.0f, 0.0f,         \
                width, -height, 1.0f, 1.0f,          \
                -width, -height, 0.0f, 1.0f,         \
                -width,  height, 0.0f, 0.0f}

#define HS_SCALED_TEX_SQUARE(scale) {                   \
                0.5f,  0.5f, 0.0f,  scale, scale,       \
                0.5f, -0.5f, 0.0f,  scale, 0.0f,        \
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,         \
                -0.5f,  0.5f, 0.0f, 0.0f, scale}

#define HS_SCALED_TEX_SQUARE_SCALED(scale, scale_cube) {            \
                scale_cube,  scale_cube, 0.0f,  scale, scale,       \
                scale_cube, -scale_cube, 0.0f,  scale, 0.0f,        \
                -scale_cube, -scale_cube, 0.0f, 0.0f, 0.0f,         \
                -scale_cube,  scale_cube, 0.0f, 0.0f, scale}

#define HS_DEFAULT_CUBE {                               \
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,       \
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f,        \
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        \
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        \
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,       \
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,       \
                                                        \
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,       \
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,        \
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,        \
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,        \
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,       \
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,       \
                                                        \
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,       \
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,       \
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,       \
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,       \
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,       \
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,       \
                                                        \
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        \
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        \
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        \
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        \
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        \
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        \
                                                        \
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,       \
                0.5f, -0.5f, -0.5f,  1.0f, 1.0f,        \
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,        \
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,        \
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,       \
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,       \
                                                        \
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,       \
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        \
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        \
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        \
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,       \
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f}

static const char* texture_transform_vert =
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "out vec2 TexCoord;\n"
        "uniform mat4 u_transform;\n"
        "void main()\n"
        "{\n"
        "gl_Position = u_transform * vec4(aPos, 0.0f, 1.0);\n"
        "TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
        "}";

static const char* texture_transform_frag =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "in vec2 TexCoord;\n"
        "uniform sampler2D u_tex;\n"
        "void main()\n"
        "{\n"
        "FragColor = texture(u_tex, TexCoord);\n"
        "}";