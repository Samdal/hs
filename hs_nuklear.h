#ifndef HS_NUKLEAR_H_
#include "hs_graphics.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "external/nuklear/nuklear.h"
#include "external/nuklear_glfw_gl3.h"

#ifndef NO_STBI
extern struct nk_image hs_nk_image_load(const char *filename);
extern struct nk_image hs_nk_image_load_size_info(const char *filename, int* width, int* height);
#endif // NO_STBI

#ifdef HS_IMPL
#define HS_NUKLEAR_IMPL
#endif //HS_IMPL

#ifdef HS_NUKLEAR_IMPL

#ifndef NO_STBI
inline struct nk_image
hs_nk_image_load(const char *filename)
{
    return nk_image_id(hs_tex2d_create(filename, GL_RGBA, GL_CLAMP_TO_EDGE, GL_NEAREST));
}

inline struct nk_image
hs_nk_image_load_size_info(const char *filename, int* width, int* height)
{
    return nk_image_id(hs_tex2d_create_size_info(filename, GL_RGBA, GL_CLAMP_TO_EDGE, GL_NEAREST, width, height));
}
#endif // NO_STBI

#endif // HS_NUKLEAR_IMPL

#define HS_NUKLEAR_H_
#endif // HS_NUKLEAR_H_
