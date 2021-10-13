#ifndef HS_GRAPHICS_H_
#define HS_GRAPHICS_H_

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef WIN32
#define OEMRESOURCE
#include <windows.h>
#endif

#include "hs_math.h"
#include "hs_data.h"
#include "hs_util.h"

#define STBI_NO_SIMD

#ifndef NO_STBI
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"
#endif

#include "external/glad/glad.h"
#include "external/glfw/glfw3.h"

#ifdef HS_SFD
#include "external/sfd/src/sfd.c"
#endif

#ifdef HS_NUKLEAR
#include "hs_nuklear.h"
#endif

#define hs_key_init(glfw_key) &(hs_key){.key = glfw_key}

typedef struct {
        uint32_t count, vbo, vao, ebo;
} hs_vobj;

typedef uint32_t hs_tex;

typedef struct {
        uint32_t model, view, proj;
} hs_coord;

typedef struct {
        uint32_t p;
        hs_vobj vobj;
        hs_coord coord;
} hs_shader_program;

typedef struct {
        vec3 pos, up, front;
        float yaw, pitch, roll;
} hs_camera;

typedef struct {
        vec2 curr, goal;
        float zoom;
} hs_camera2_smooth;

typedef struct {
        vec2 pos, tex;
} hs_tex_corner;

typedef struct {
         hs_tex_corner c[6];
} hs_tex_square;

typedef struct {
        uint32_t width, height, tileset_width, tileset_height;
        float tile_width, tile_height;
        hs_shader_program sp;
        hs_tex tex;
        hs_tex_square* vertices;
} hs_tilemap;

typedef struct {
        uint32_t tileset_width, tileset_height;
        float tile_width, tile_height;
        hs_shader_program sp;
        hs_tex tex;
        hs_dynarr vertices;
} hs_dyn_tilemap;

// this is how anders tale rooms are stored
// --LAYERS MUST BE AT LEAST 1--
typedef struct {
        uint16_t width, height, layers;
        uint8_t* data;
} hs_aroom;

typedef struct {
        uint32_t width, height;
        GLFWwindow* window;
} hs_game_data;

typedef struct {
        vec2 tr, bl;
} hs_aabb2;

typedef struct {
        vec2i tr, bl;
} hs_aabb2i;

typedef struct {
        vec2 pos, half_size;
} hs_rect2;

typedef struct {
        hs_rect2 r;
        uint32_t flags;
        hs_tex tex;
        hs_shader_program sp;
} hs_entity2_hot;

typedef struct {
        vec2 external_velocity;
        float base_mov_speed, mov_speed_mul, fire_rate_mul, invisframe_mul;
        uint32_t max_hp, hp, armour;
        void* current_room; //TODO: create room struct and stuff
} hs_entity2_cold;

typedef struct {
        hs_entity2_hot* hot;
        hs_entity2_cold cold;
} hs_entity2;

enum entity2_flags {
        AABB_STATIC = 1 << 0,
        AABB_RIGID = 1 << 1,
        //AABB_CHARACTHER = 1 << 2,
        PLAYER = 1 << 3,
        PREBLINK = 1 << 4,
        INVISFRAME = 1 << 5,
        CONFUSION = 1 << 6,
        STUNNED = 1 << 7,
        DISABLED = 1 << 8,
};

enum hs_key_state {
        HS_KEY_UP = 0,
        HS_KEY_DOWN = 1,
        HS_KEY_RELEASED = 2,
        HS_KEY_PRESSED = 3,
};

typedef struct {
        const int key;
        int previous_state;
} hs_key;

/* Keys */
extern enum hs_key_state hs_get_key_toggle(const hs_game_data gd,   hs_key* key);
extern enum hs_key_state hs_get_mouse_toggle(const hs_game_data gd, hs_key* key);
extern enum hs_key_state hs_get_key_held(const hs_game_data gd,   const int key);
extern enum hs_key_state hs_get_mouse_held(const hs_game_data gd, const int key);

/* MISC */
extern void     hs_close(const hs_game_data gd);
extern float    hs_delta();
extern void     hs_sp_use(const hs_shader_program sp);
extern int32_t  hs_window_up(const hs_game_data gd);
extern void     hs_clear(const float r, const  float g, const  float b, const  float a, const GLbitfield mask);
extern void     hs_avg_frametime_print(const float delta, const float interval);
extern void     hs_avg_fps_print(const float delta, const float interval);
extern void     hs_fps_callback_init(const hs_game_data gd, void(*mouse_callback)(GLFWwindow*, double xpos, double ypos));

/* Vertex Buffer */
extern uint32_t hs_vao_create(const uint32_t  count);
extern uint32_t hs_vbo_create(const float    *vbuff, const uint32_t buffsize,
                              const GLenum    usage, const uint32_t count);
extern uint32_t hs_ebo_create(const uint32_t *ibuff, const uint32_t buffsize,
                              const GLenum    usage, const uint32_t count);
extern hs_vobj  hs_vobj_create(const float    *vbuff, const uint32_t vbuffsize,
                              const uint32_t *ibuff, const uint32_t ibuffsize,
                              const GLenum    usage, const uint32_t count);
extern void     hs_vobj_free(hs_vobj vobj);
extern void     hs_vattrib_enable(const uint32_t index, const uint32_t size, const GLenum type,
                                  const uint32_t stride, const size_t pointer);
extern void     hs_vattrib_enable_float(const uint32_t index, const uint32_t size,
                                         const uint32_t stride, const size_t pointer);

/* Shader program */
extern hs_shader_program hs_shader_program_create(const uint32_t sp, hs_vobj vobj);
extern uint32_t          hs_shader_create(const char *src, const GLenum shader_type);
extern uint32_t          hs_sp_create(const uint32_t v_shader, const uint32_t f_shader);
extern void              hs_sp_delete(hs_shader_program sp);
extern uint32_t          hs_sp_texture_transform_create();
extern uint32_t hs_sp_create_from_src(const char *v_src,   const char *f_src);
extern uint32_t hs_sp_create_from_file(const char *v_file, const char *f_file);

/* Uniforms */
extern uint32_t hs_uniform_create(const uint32_t program, const char* name);
extern hs_coord hs_uniform_coord_create(const uint32_t program, const char* model, const char* view, const char* proj);
extern void     hs_uniform_mat4_set(const uint32_t u_mat, const mat4 mat);
extern void     hs_uniform_sp_mat4_set(const uint32_t program, const uint32_t u_mat, const mat4 mat);
extern void     hs_uniform_vec2_set(const uint32_t u_vec, const vec2 vec);
extern void     hs_uniform_sp_vec2_set(const uint32_t program, const uint32_t u_vec, const vec2 vec);
extern void     hs_tex_uniform_set(const hs_tex tex, const uint32_t val);
extern void     hs_tex2d_activate(const uint32_t texture_object, const GLenum texindex);
#ifndef NO_STBI
extern uint32_t hs_tex2d_create(const char *filename, const GLenum format, const GLenum  wrap, const GLenum filter);
extern uint32_t hs_tex2d_create_pixel(const char *filename, const GLenum format);
extern uint32_t hs_tex2d_create_size_info(const char *filename, const GLenum format, const GLenum wrap, const GLenum filter, int* width, int* height);
extern uint32_t hs_tex2d_create_size_info_pixel(const char *filename, const GLenum format, int* width, int* height);
#endif

/* FBO */
extern uint32_t hs_fbo_color_create(const uint32_t width,  const uint32_t height, uint32_t* tex);
extern void     hs_fbo_draw_to_screen(const uint32_t fbo,  const uint32_t src_w, const uint32_t src_h,
                      const uint32_t dst_x, const uint32_t dst_y, const uint32_t dst_w, const uint32_t dst_h);

/* hs_aabb2 */
extern vec2     hs_aabb2_center(const hs_aabb2 rect);
extern vec2     hs_aabb2_size(const hs_aabb2 rect);
extern vec2     hs_aabb2_half_size(const hs_aabb2 rect);
extern vec2i    hs_aabb2i_center(const hs_aabb2i rect);
extern vec2i    hs_aabb2i_size(const hs_aabb2i rect);
extern vec2i    hs_aabb2i_half_size(const hs_aabb2i rect);
extern hs_aabb2 hs_aabb2_from_rect2(const hs_rect2 r);

/* Anders Tale Dungeon generation v3 (BSP) */
extern uint32_t hs_bsp_recti_split_in_place_append(hs_aabb2i* rects, const uint32_t new_rect_index, const vec2i min_rect_size);

/* Physics */
extern uint32_t hs_rect2_is_inside(const hs_rect2 r1, const hs_rect2 r2, float* lenabs);
extern void     hs_entity2_force_inside_rects(hs_rect2* e, hs_rect2* rects, const uint32_t rectc);
extern void     hs_entity2_collide(hs_rect2* r1, const hs_rect2* r2);

/* Camera */
extern hs_camera hs_init_fps_camera();
extern void      hs_camera_move_front(hs_camera* camera, const float scale);
extern void      hs_camera_move_right(hs_camera* camera, const float scale);
extern void      hs_camera_move_frontv(const hs_camera camera, vec3* new_pos);
extern void      hs_camera_move_backv(const hs_camera camera, vec3* new_pos);
extern void      hs_camera_move_rightv(const hs_camera camera, vec3* new_pos);
extern void      hs_camera_move_leftv(const hs_camera camera, vec3* new_pos);
extern void      hs_camera_look_at(mat4 view, const hs_camera camera);
extern vec2      hs_pos_to_offset(const float xpos, const float ypos, const float sens);
extern void      hs_camera_update_front(hs_camera* camera);

#define HS_CAMERA2_SMOOTH_DEFAULT {.zoom = 1.0f}
extern void hs_camera2_smooth_view(mat4 view, const hs_camera2_smooth cam);
extern void hs_camera2_smooth_move_to_goal(hs_camera2_smooth* cam, const float scale);
extern vec2 hs_px_coord_to_global(const vec2 cam_offset, const vec2 scale, const hs_aabb2 res, const vec2 px);

/* Tilemap */
extern hs_aroom hs_aroom_from_file(const char* file_path);
extern void     hs_aroom_write_to_file(const char* file_path, const hs_aroom aroom);
extern void     hs_tex_square_set_pos(hs_tex_square* square, const hs_aabb2 pos);
extern void     hs_tex_square_set_tex(hs_tex_square* square, hs_aabb2 tex);

extern void     hs_tilemap_set(hs_tilemap* tilemap, const uint32_t vertex, uint32_t tile);
extern void     hs_tilemap_setall(hs_tilemap* tilemap, const uint32_t tile);
extern void     hs_tilemap_set_xy(hs_tilemap* tilemap, const uint32_t x, const uint32_t y, uint32_t tile);
extern uint32_t hs_tilemap_sizeof(const hs_tilemap tilemap);
extern void     hs_tilemap_init(hs_tilemap* tilemap, const uint32_t default_tex);
extern void     hs_tilemap_update_vbo(const hs_tilemap tilemap);
extern void     hs_tilemap_draw(const hs_tilemap tilemap);
extern void     hs_tilemap_free(hs_tilemap* tilemap);
extern void     hs_aroom_set_xy(hs_aroom* aroom, const uint16_t x, const uint16_t y, const uint16_t data);
extern void     hs_aroom_to_tilemap(const hs_aroom aroom, hs_tilemap* tilemap, const uint16_t layer);
extern uint8_t  hs_aroom_get_xy(const hs_aroom aroom, const uint16_t x, const uint16_t y);
extern void     hs_aroom_set_tilemap(const hs_aroom aroom, hs_tilemap* tilemap, const uint16_t layer);
extern void     hs_aroom_set_tilemap_offsetv(const hs_aroom aroom, hs_tilemap* tilemap, const uint16_t layer, const vec2i offset);
extern vec2     hs_tilemap_pos_to_global(const hs_tilemap tilemap, vec2i pos);

extern uint32_t hs_dyn_tilemap_sizeof(const hs_dyn_tilemap tilemap);
extern void     hs_dyn_tilemap_init(hs_dyn_tilemap* tilemap, size_t size);
extern void     hs_dyn_tilemap_clear(hs_dyn_tilemap* tilemap);
extern void     hs_dyn_tilemap_free(hs_dyn_tilemap* tilemap);
extern void     hs_dyn_tilemap_push(hs_dyn_tilemap* tilemap, const vec2i pos, uint32_t tile);
extern void     hs_aroom_push_dyn_tilemap(const hs_aroom aroom, hs_dyn_tilemap* tilemap, const uint16_t layer, const vec2i offset);
extern void     hs_dyn_tilemap_update_vbo(const hs_dyn_tilemap tilemap);
extern void     hs_dyn_tilemap_draw(const hs_dyn_tilemap tilemap);
extern vec2     hs_dyn_tilemap_pos_to_global(const hs_dyn_tilemap tilemap, vec2i pos);

/* Entity */
extern hs_shader_program hs_sp_sprite_create(const float width, const float height, const float screen_size);
extern void              hs_sprite_draw_current();

extern hs_entity2 hs_entity2_create(hs_entity2_hot* hot, hs_shader_program sp, hs_tex tex);

#ifdef HS_IMPL
static uint32_t hs_default_missing_tex = 0;

#define GLAD_IMPL
#include "external/glad/glad_impl.h"
#define GLFW_IMPL
#include "external/glfw/glfw_impl.h"

#define hs_loop(game_data, update_func) while(hs_window_up(game_data)) {hs_poll_input(); update_func; hs_end_frame(game_data);}

inline void
hs_clear(const float r, const  float g, const  float b, const  float a, const GLbitfield mask)
{
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | mask);
}

inline uint32_t
hs_uniform_create(const uint32_t program, const char *name)
{
        return glGetUniformLocation(program, name);
}

inline hs_coord
hs_uniform_coord_create(const uint32_t program,
                        const char* model, const char* view, const char* proj)
{
        const uint32_t u_model = glGetUniformLocation(program, model);
        const uint32_t u_view =  glGetUniformLocation(program, view);
        const uint32_t u_proj =  glGetUniformLocation(program, proj);

        return (hs_coord) {
                .model = u_model,
                .view  = u_view,
                .proj  = u_proj,
        };
}

void
hs_uniform_mat4_set(const uint32_t u_mat, const mat4 mat)
{
        glUniformMatrix4fv(u_mat, 1, GL_FALSE, castf(mat));
}

void
hs_uniform_sp_mat4_set(const uint32_t program, const uint32_t u_mat, const mat4 mat)
{
        glUseProgram(program);
        glUniformMatrix4fv(u_mat, 1, GL_FALSE, castf(mat));
}

void
hs_uniform_vec2_set(const uint32_t u_vec, const vec2 vec)
{
        glUniform2fv(u_vec, 1, vec.xy);
}

void
hs_uniform_sp_vec2_set(const uint32_t program, const uint32_t u_vec, const vec2 vec)
{
        glUseProgram(program);
        glUniform2fv(u_vec, 1, vec.xy);
}

inline enum hs_key_state
hs_get_key_toggle(const hs_game_data gd, hs_key* key)
{
        const int state = glfwGetKey(gd.window, key->key);
        const int previous_state = key->previous_state;
        key->previous_state = state;

        if (previous_state != state) {
                if (state == GLFW_PRESS) return HS_KEY_PRESSED;
                else                     return HS_KEY_RELEASED;
        }

        return HS_KEY_UP;
}

inline enum hs_key_state
hs_get_key_held(const hs_game_data gd, const int key)
{
        const int state = glfwGetKey(gd.window, key);
        if (state == GLFW_PRESS)
                return HS_KEY_DOWN;
        return HS_KEY_UP;
}

inline enum hs_key_state
hs_get_mouse_toggle(const hs_game_data gd, hs_key* key)
{
        const int state = glfwGetMouseButton(gd.window, key->key);
        const int previous_state = key->previous_state;
        key->previous_state = state;

        if (previous_state != state) {
                if (state == GLFW_PRESS) return HS_KEY_PRESSED;
                else                     return HS_KEY_RELEASED;
        }

        return HS_KEY_UP;
}

inline enum hs_key_state
hs_get_mouse_held(const hs_game_data gd, const int key)
{
        const int state = glfwGetMouseButton(gd.window, key);
        if (state == GLFW_PRESS)
                return HS_KEY_DOWN;
        return HS_KEY_UP;
}

inline int
hs_window_up(const hs_game_data gd)
{
        return !glfwWindowShouldClose(gd.window);
}

inline void
hs_close(const hs_game_data gd)
{
        return glfwSetWindowShouldClose(gd.window, true);
}

inline void
hs_vattrib_enable(const uint32_t index, const uint32_t size,
                  const GLenum type, const uint32_t stride, const size_t pointer)
{
        glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void*)pointer);
        glEnableVertexAttribArray(index);
}

inline void
hs_vattrib_enable_float(const uint32_t index, const uint32_t size,
                        const uint32_t stride, const size_t pointer)
{
        hs_vattrib_enable(index, size, GL_FLOAT, stride * sizeof(float), pointer * sizeof(float));
}

inline float
hs_delta()
{
        static float last_frame = 0.0f;
        float current_frame = glfwGetTime() * 1000.0f;
        float delta = current_frame - last_frame;

        last_frame = current_frame;
        return delta;
}

inline void
hs_sp_use(const hs_shader_program sp)
{
        glUseProgram(sp.p);
        glBindVertexArray(sp.vobj.vao);
        if (sp.vobj.ebo) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sp.vobj.ebo);
}

hs_aroom
hs_aroom_from_file(const char* file_path)
{
        uint8_t* data = hs_file_read(file_path);
        uint16_t* header = (uint16_t*)data;
        hs_aroom aroom;
        aroom.width = header[0];
        aroom.height = header[1];
        aroom.layers = header[2] == 0 ? 1 : header[2];

        uint8_t* aroom_data = (uint8_t*)malloc(aroom.width * aroom.height * aroom.layers);
        assert(aroom_data);
        memcpy(aroom_data, &data[sizeof(uint16_t)*3], aroom.width * aroom.height * aroom.layers);
        free(data);

        aroom.data = aroom_data;
        return aroom;
}

void
hs_aroom_write_to_file(const char* file_path, const hs_aroom aroom)
{
        uint16_t layers = aroom.layers == 0 ? 1 : aroom.layers;
        FILE *file = fopen(file_path, "wb");
        if (!file) {
                fprintf(stderr, "---error writing to file \"%s\"---\n", file_path);
                assert(file);
        }

        fwrite(&aroom.width, sizeof(uint16_t), 1, file);
        fwrite(&aroom.height, sizeof(uint16_t), 1, file);
        fwrite(&layers, sizeof(uint16_t), 1, file);
        fwrite(aroom.data, aroom.width * aroom.height * layers, 1, file);
        fclose(file);
}

uint32_t
hs_shader_create(const char *src, const GLenum shader_type)
{
        uint32_t shader = glCreateShader(shader_type);
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        int  shader_compile_success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_compile_success);

        if (!shader_compile_success) {
                char info_log[512];
                glGetShaderInfoLog(shader, 512, NULL, info_log);
                fprintf(stderr, "-------------ERROR------------\n"
                       "::OpenGL Failed to compile shader::\n%s\n", info_log);
                fprintf(stderr, "-------------SOURCE------------\n");
                fprintf(stderr, "%s\n", src);
                fprintf(stderr, "\n------------END_SOURCE----------\n");
                assert(shader_compile_success);
        }

        return shader;
}

uint32_t
hs_sp_create(const uint32_t v_shader, const uint32_t f_shader)
{
        uint32_t program = glCreateProgram();
        glAttachShader(program, v_shader);
        glAttachShader(program, f_shader);
        glLinkProgram(program);

        int  program_link_success;
        glGetProgramiv(program, GL_LINK_STATUS, &program_link_success);

        if (!program_link_success) {
                char info_log[512];
                glGetProgramInfoLog(program, 512, NULL, info_log);
                fprintf(stderr, "-------------ERROR------------\n"
                       "::OpenGL Failed to link program::\n%s\n", info_log);
                assert(program_link_success);
        }

        glDeleteShader(v_shader);
        glDeleteShader(f_shader);

        glUseProgram(program);
        return program;
}

inline uint32_t
hs_sp_texture_transform_create()
{
        return hs_sp_create_from_src(texture_transform_vert, texture_transform_frag);
}

inline uint32_t
hs_sp_create_from_src(const char *v_src, const char *f_src)
{
        uint32_t v_shader = hs_shader_create(v_src, GL_VERTEX_SHADER);
        uint32_t f_shader = hs_shader_create(f_src, GL_FRAGMENT_SHADER);

        return hs_sp_create(v_shader, f_shader);
}

uint32_t
hs_sp_create_from_file(const char *v_file, const char *f_file)
{
        char* v_src = hs_file_read_null_term(v_file);
        char* f_src = hs_file_read_null_term(f_file);

        uint32_t sp = hs_sp_create_from_src(v_src, f_src);

        free(v_src);
        free(f_src);

        glUseProgram(sp);
        return sp;
}

inline uint32_t
hs_fbo_color_create(const uint32_t width, const uint32_t height, uint32_t* tex)
{
        uint32_t fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

        glGenTextures(1, tex);
        glBindTexture(GL_TEXTURE_2D, *tex);

        // create attached texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *tex, 0);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        return fbo;
}

inline void
hs_fbo_draw_to_screen(const uint32_t fbo, const uint32_t src_w, const uint32_t src_h,
                      const uint32_t dst_x, const uint32_t dst_y, const uint32_t dst_w, const uint32_t dst_h)
{
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        hs_clear(0.0f, 0.0f, 0.0f, 1.0f, 0);

        glBlitFramebuffer(0, 0, src_w, src_h, dst_x, dst_y, dst_w, dst_h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

inline hs_shader_program
hs_shader_program_create(const uint32_t sp, hs_vobj vobj)
{
        return (hs_shader_program) {
                .p = sp,
                .vobj = vobj,
        };
}

inline void
hs_sp_delete(hs_shader_program sp)
{
        glDeleteProgram(sp.p);
        hs_vobj_free(sp.vobj);
}

inline void
hs_avg_frametime_print(const float delta, const float interval)
{
        static float prev = 0.0f;
        static float acum = 0.0f;
        static uint32_t times = 0;
        prev += delta;
        acum += delta;
        times++;

        if (prev > interval) {
                printf("avg frametime: %f\n", (acum/times));
                prev = 0.0f;
                acum = 0.0f;
                times = 0;
        }
}

inline void
hs_avg_fps_print(const float delta, const float interval)
{
        static float prev = 0.0f;
        static float acum = 0.0f;
        static uint32_t times = 0;
        prev += delta;
        acum += delta;
        times++;

        if (prev > interval) {
                printf("avg fps: %f\n", 1000/(acum/times));
                prev = 0.0f;
                acum = 0.0f;
                times = 0;
        }
}

inline void
hs_tex_uniform_set(const uint32_t u_tex, const uint32_t tex)
{
        glUniform1i(u_tex, tex);
}

inline void
hs_tex2d_activate(const uint32_t texture_object, const GLenum texindex)
{
        glActiveTexture(texindex);
        glBindTexture(GL_TEXTURE_2D, texture_object);
}

#ifndef NO_STBI
uint32_t
hs_tex2d_create(const char *filename, const GLenum format,
                const GLenum wrap, const GLenum filter)
{
        uint32_t tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

        int width, height, nr_channels;
        unsigned char* texture_data = stbi_load(filename, &width, &height, &nr_channels, 0);
        if (!texture_data) {
                fprintf(stderr, "---error loading texture \"%s\"--\n", filename);
                assert(texture_data);
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(texture_data);

        return tex;
}

inline uint32_t
hs_tex2d_create_pixel(const char *filename, const GLenum format)
{
        return hs_tex2d_create(filename, format, GL_REPEAT, GL_NEAREST);
}

inline uint32_t
hs_tex2d_create_size_info_pixel(const char *filename, const GLenum format, int* width, int* height)
{
        return hs_tex2d_create_size_info(filename, format, GL_REPEAT, GL_NEAREST, width, height);
}

uint32_t
hs_tex2d_create_size_info(const char *filename, const GLenum format,
                          const GLenum  wrap, const GLenum filter,
                          int* width, int* height)
{
        uint32_t tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

        int nr_channels;
        unsigned char* texture_data = stbi_load(filename, width, height, &nr_channels, 0);
        if (!texture_data) {
                fprintf(stderr, "---error loading texture \"%s\"--\n", filename);
                assert(texture_data);
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, *width, *height, 0, format, GL_UNSIGNED_BYTE, texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(texture_data);

        return tex;
}
#endif // NO_STBI

inline hs_camera
hs_init_fps_camera()
{
        return (hs_camera){
                .up = {0.0f, 1.0f, 0.0f},
                .front = {0.0f, 0.0f, -1.0f},
                .yaw = -90.0f
        };
}

inline vec2
hs_aabb2_center(const hs_aabb2 rect)
{
        return vec2_add(rect.bl, hs_aabb2_half_size(rect));
}

inline vec2i
hs_aabb2i_center(const hs_aabb2i rect)
{
        return vec2i_add(rect.bl, hs_aabb2i_half_size(rect));
}

inline vec2
hs_aabb2_size(const hs_aabb2 rect)
{
        return vec2_sub(rect.tr, rect.bl);
}

inline vec2
hs_aabb2_half_size(const hs_aabb2 rect)
{
        return vec2_scale(vec2_sub(rect.tr, rect.bl), 0.5f);
}

inline vec2i
hs_aabb2i_size(const hs_aabb2i rect)
{
        return vec2i_sub(rect.tr, rect.bl);
}

inline vec2i
hs_aabb2i_half_size(const hs_aabb2i rect)
{
        return vec2i_div(vec2i_sub(rect.tr, rect.bl), 2);
}

inline hs_aabb2
hs_aabb2_from_rect2(const hs_rect2 r)
{
        return(hs_aabb2){
                .bl = vec2_sub(r.pos, r.half_size),
                .tr = vec2_add(r.pos, r.half_size),
        };
}

inline uint32_t
hs_bsp_recti_split_in_place_append(hs_aabb2i* rects, const uint32_t new_rect_index, const vec2i min_rect_size)
{
        // this way of checking if the room is too small does not respect very non-square rectangles
        // TOOD: maybe change this^

        uint32_t rect_index;
        for (uint32_t tries = 0; tries < 7; tries++) {
                rect_index = rand() % new_rect_index;

                // half rect size instead of size since we are trying to split it
                vec2i half_rect_size = hs_aabb2i_half_size(rects[rect_index]);
                if (min_rect_size.x <= half_rect_size.x && min_rect_size.y <= half_rect_size.y)
                        goto random_rect_found;
        }

        // randomly trying rooms failed,
        // looping through all rooms to try and find a suitable room
        for (rect_index = 0; rect_index < new_rect_index; rect_index++) {

                vec2i half_rect_size = hs_aabb2i_half_size(rects[rect_index]);
                if (min_rect_size.x <= half_rect_size.x && min_rect_size.y <= half_rect_size.y)
                        goto random_rect_found;
        }

        return false;

        random_rect_found:

        rects[new_rect_index] = rects[rect_index];
        const vec2i rect_size = hs_aabb2i_size(rects[rect_index]);

        // TODO maybe add some randomness to this
        const uint32_t axis = rect_size.x >= rect_size.y ? 0 : 1;

        const int32_t axis_half_size = rect_size.xy[axis] / 2;
        const int32_t axis_center = rects[rect_index].bl.xy[axis] + axis_half_size;
        const int32_t max_split = axis_half_size - min_rect_size.xy[axis];

        int32_t split = 0;
        if (max_split)
                split = (rand() % (max_split * 2)) - max_split;

        rects[rect_index].bl.xy[axis] = axis_center - split;
        rects[new_rect_index].tr.xy[axis] = axis_center - split - 1;
        return true;
}

inline uint32_t
hs_rect2_is_inside(const hs_rect2 r1, const hs_rect2 r2, float* lenabs)
{
        const vec2 distance = vec2_add(r1.half_size, r2.half_size);
        const vec2 diff = vec2_sub(r1.pos, r2.pos);
        const vec2 diffabs = {fabs(diff.x), fabs(diff.y)};
        *lenabs = vec2_len(diffabs);

        if (diffabs.x >= distance.x || diffabs.y >= distance.y)
                return false;
        return true;
}

void
hs_entity2_force_inside_rects(hs_rect2* e, hs_rect2* rects, const uint32_t rectc)
{
        float len_to_closest_rect = INFINITY;
        uint32_t closest_rect = 0;
        for (uint32_t i = 0; i < rectc; i++) {
                float len;
                hs_rect2 r = rects[i];
                r.half_size = vec2_sub(r.half_size, vec2_scale(e->half_size, 2.0f));
                if (hs_rect2_is_inside(*e, r, &len)) return;

                if (len < len_to_closest_rect) {
                        closest_rect = i;
                        len_to_closest_rect = len;
                }
        }

        /* move player inside closest room */

        const vec2 distance = vec2_sub(rects[closest_rect].half_size, e->half_size);
        vec2 diff = vec2_sub(e->pos, rects[closest_rect].pos);
        const vec2 diffabs = {fabs(diff.x), fabs(diff.y)};
        const float extra = 1.0f - 0.001f;

        if (diffabs.x > diffabs.y) {
                const float mul = (diff.x / diffabs.x) * extra;
                CLAMP(diff.x, distance.x, -distance.x);
                e->pos = vec2_add(rects[closest_rect].pos, (vec2){distance.x * mul, diff.y});
        } else {
                const float mul = (diff.y / diffabs.y) * extra;
                CLAMP(diff.y, distance.y, -distance.y);
                e->pos = vec2_add(rects[closest_rect].pos, (vec2){diff.x, distance.y * mul});
        }
}

inline void
hs_entity2_collide(hs_rect2* r1, const hs_rect2* r2)
{
        const vec2 distance = vec2_add(r1->half_size, r2->half_size);
        vec2 diff = vec2_sub(r1->pos, r2->pos);

        if (fabs(diff.x) >= distance.x || fabs(diff.y) >= distance.y) return;

        if (diff.x == 0) diff.x = random_float_negative() * 0.00000001;
        if (diff.y == 0) diff.y = random_float_negative() * 0.00000001;
        const vec2 diffabs = {fabs(diff.x), fabs(diff.y)};

        const float extra = 1.0001;

        if (diffabs.x > diffabs.y) {
                const float mul = (diff.x / diffabs.x) * extra;
                r1->pos = vec2_add(r2->pos, (vec2){distance.x * mul, diff.y});
        } else {
                const float mul = (diff.y / diffabs.y) * extra;
                r1->pos = vec2_add(r2->pos, (vec2){diff.x, distance.y * mul});
        }
        //static uint32_t col = 0;
        //printf("collisions %d\n", col++);
}

inline void
hs_camera_move_front(hs_camera* camera, const float scale)
{
        camera->pos = vec3_add(camera->pos, vec3_scale(camera->front, scale));
}

inline void
hs_camera_move_right(hs_camera* camera, const float scale)
{
        camera->pos = vec3_add(camera->pos, vec3_scale(vec3_norm(vec3_cross(camera->front, camera->up)), scale));
}

inline void
hs_camera_move_frontv(const hs_camera camera, vec3* new_pos)
{
        *new_pos = vec3_add(*new_pos, camera.front);
}

inline void
hs_camera_move_rightv(const hs_camera camera, vec3* new_pos)
{
        *new_pos = vec3_add(*new_pos, vec3_norm(vec3_cross(camera.front, camera.up)));
}

inline void
hs_camera_move_backv(const hs_camera camera, vec3* new_pos)
{
        *new_pos = vec3_sub(*new_pos, camera.front);
}

inline void
hs_camera_move_leftv(const hs_camera camera, vec3* new_pos)
{
        *new_pos = vec3_sub(*new_pos, vec3_norm(vec3_cross(camera.front, camera.up)));
}

inline void
hs_camera_look_at(mat4 view, const hs_camera camera)
{
        mat4_look_at(view, camera.pos,
                     vec3_add(camera.pos, camera.front),
                     camera.up);
}

inline vec2
hs_pos_to_offset(const float xpos, const float ypos, const float sens)
{
        static bool first_mouse = true;
        static float last_x = 400;
        static float last_y = 300;

        if (first_mouse) {
                last_x = xpos;
                last_y = ypos;
                first_mouse = false;
        }
        float xoffset = xpos - last_x;
        float yoffset = last_y - ypos;
        last_x = xpos;
        last_y = ypos;

        return (vec2){
                xoffset * sens,
                yoffset * sens,
        };
}

inline void
hs_camera_update_front(hs_camera* camera)
{
        camera->front = vec3_norm((vec3){
                cosf(deg2rad(camera->yaw)) * cosf(deg2rad(camera->pitch)),
                sinf(deg2rad(camera->pitch)),
                sinf(deg2rad(camera->yaw)) * cosf(deg2rad(camera->pitch)),
        });
}

inline void
hs_camera2_smooth_move_to_goal(hs_camera2_smooth* cam, const float scale)
{
        cam->curr = vec2_add(cam->curr, vec2_scale(vec2_sub(cam->goal, cam->curr), scale));
}

vec2
hs_px_coord_to_global(const vec2 cam_offset, const vec2 scale, const hs_aabb2 res, const vec2 px)
{
        if (px.x < res.bl.x || px.y < res.bl.y) return (vec2){NAN, NAN};
        if (px.x > res.tr.x || px.y > res.tr.y) return (vec2){NAN, NAN};

        vec2 global;
        global.x = MAP(px.x, res.bl.x, res.tr.x, -1/scale.x, 1/scale.x);
        global.y = MAP(px.y, res.bl.y, res.tr.y, -1/scale.y, 1/scale.y);

        return vec2_add(global, cam_offset);
}

inline void
hs_camera2_smooth_view(mat4 view, const hs_camera2_smooth cam)
{
        mat4_translate(view, (vec3){-cam.curr.x, -cam.curr.y, 0.0f});
}

inline void
hs_tex_square_set_pos(hs_tex_square* square, const hs_aabb2 pos)
{
        // triangle one
        square->c[0].pos.x = pos.bl.x;
        square->c[0].pos.y = pos.bl.y;

        square->c[1].pos.x = pos.tr.x;
        square->c[1].pos.y = pos.bl.y;

        square->c[2].pos.x = pos.tr.x;
        square->c[2].pos.y = pos.tr.y;

        // triangle two
        square->c[3].pos.x = pos.tr.x;
        square->c[3].pos.y = pos.tr.y;

        square->c[4].pos.x = pos.bl.x;
        square->c[4].pos.y = pos.tr.y;

        square->c[5].pos.x = pos.bl.x;
        square->c[5].pos.y = pos.bl.y;
}

void
hs_tex_square_set_tex(hs_tex_square* square, hs_aabb2 tex)
{
        // crop it slightly, this makes the tiles next to the selected tile not accidentaly appear
        tex.bl.x += 0.001f;
        tex.bl.y += 0.001f;
        tex.bl.x -= 0.001f;
        tex.tr.y -= 0.001f;

        // bottom left
        square->c[0].tex.x = tex.bl.x;
        square->c[0].tex.y = tex.bl.y;

        // bottom right
        square->c[1].tex.x = tex.tr.x;
        square->c[1].tex.y = tex.bl.y;

        // top right
        square->c[2].tex.x = tex.tr.x;
        square->c[2].tex.y = tex.tr.y;

        // top right
        square->c[3].tex.x = tex.tr.x;
        square->c[3].tex.y = tex.tr.y;

        // top left
        square->c[4].tex.x = tex.bl.x;
        square->c[4].tex.y = tex.tr.y;

        // bottom left
        square->c[5].tex.x = tex.bl.x;
        square->c[5].tex.y = tex.bl.y;
}

inline void
hs_tilemap_set(hs_tilemap* tilemap, const uint32_t vertex, uint32_t tile)
{
        // make tiles start at 0 instead of 1
        tile++;

        const float width      = 1.0f/tilemap->tileset_width;
        const float height     = 1.0f/tilemap->tileset_height;
        const float ypos       = height * ceilf((float)tile / (float)tilemap->tileset_width);
        float xpos             = width * (tile % tilemap->tileset_width);
        if (xpos == 0.0f) xpos = 1.0f;

        hs_aabb2 tex = {
                .bl = (vec2){xpos - width, ypos - height},
                .tr = (vec2){xpos, ypos},
        };

        hs_tex_square_set_tex(&tilemap->vertices[vertex], tex);
}

void
hs_tilemap_setall(hs_tilemap* tilemap, const uint32_t tile)
{
        const uint32_t tilemap_size = tilemap->width * tilemap->height;
        for(uint32_t v = 0; v < tilemap_size; v++)
                hs_tilemap_set(tilemap, v, tile);
}

inline void
hs_tilemap_set_xy(hs_tilemap* tilemap, const uint32_t x, uint32_t y, uint32_t tile)
{
        hs_tilemap_set(tilemap, y * tilemap->width + x, tile);
}

inline uint32_t
hs_tilemap_sizeof(const hs_tilemap tilemap)
{
        return sizeof(hs_tex_square) * tilemap.width * tilemap.height;
}

void
hs_tilemap_init(hs_tilemap* tilemap, const uint32_t default_tex)
{
        assert(tilemap->width);
        assert(tilemap->height);
        if (tilemap->tile_width <= 0.0f)   tilemap->tile_width = 1.0f/tilemap->width;
        if (tilemap->tile_height <= 0.0f)  tilemap->tile_height = 1.0f/tilemap->height;
        if (tilemap->tileset_width == 0)   tilemap->tileset_width = 1;
        if (tilemap->tileset_height == 0)  tilemap->tileset_height = 1;
        if (tilemap->tex == 0) tilemap->tex = hs_default_missing_tex;

        if (tilemap->vertices) {
                free(tilemap->vertices);
                tilemap->vertices = NULL;
        }

        tilemap->vertices = malloc(hs_tilemap_sizeof(*tilemap));
        assert(tilemap->vertices);

        const float offset_x_default = -(float)tilemap->width * tilemap->tile_width + tilemap->tile_width;
        vec2 offset = {offset_x_default, -(float)tilemap->height * tilemap->tile_height + tilemap->tile_height};
        uint32_t vertex = 0;
        hs_aabb2 square;

        for (uint32_t y = 0; y < tilemap->height; y++) {
                square.tr.y = offset.y + tilemap->tile_height;
                square.bl.y = offset.y - tilemap->tile_height;

                for (uint32_t x = 0; x < tilemap->width; x++) {
                        square.tr.x = offset.x + tilemap->tile_width;
                        square.bl.x = offset.x - tilemap->tile_width;

                        hs_tex_square_set_pos(&tilemap->vertices[vertex], square);
                        hs_tilemap_set(tilemap, vertex, default_tex);

                        offset.x += tilemap->tile_width * 2.0f;
                        vertex++;
                }

                offset.x = offset_x_default;
                offset.y += tilemap->tile_height * 2.0f;
        }

        if (!tilemap->sp.p) {
                hs_vobj vobj = hs_vobj_create(castf(tilemap->vertices), hs_tilemap_sizeof(*tilemap), 0, 0, GL_DYNAMIC_DRAW, 1);
                tilemap->sp = hs_shader_program_create(hs_sp_texture_transform_create(), vobj);

                hs_tex_uniform_set(hs_uniform_create(tilemap->sp.p, "u_tex"), 0);
                tilemap->sp.coord = hs_uniform_coord_create(tilemap->sp.p, "u_model", "u_view", "u_proj");
                hs_uniform_mat4_set(tilemap->sp.coord.proj, (mat4)MAT4_IDENTITY);

                hs_vattrib_enable_float(0, 2, 4, 0);
                hs_vattrib_enable_float(1, 2, 4, 2);

        } else {
                hs_tilemap_update_vbo(*tilemap);
        }
}

inline void
hs_tilemap_update_vbo(const hs_tilemap tilemap)
{
        glBindBuffer(GL_ARRAY_BUFFER, tilemap.sp.vobj.vbo);
        glBufferData(GL_ARRAY_BUFFER, hs_tilemap_sizeof(tilemap), castf(tilemap.vertices), GL_DYNAMIC_DRAW);
}

inline void
hs_tilemap_draw(const hs_tilemap tilemap)
{
        hs_sp_use(tilemap.sp);
        glDrawArrays(GL_TRIANGLES, 0, 6 * tilemap.width * tilemap.height);
}

inline void
hs_tilemap_free(hs_tilemap* tilemap)
{
        free(tilemap->vertices);
        hs_sp_delete(tilemap->sp);
        glDeleteTextures(1, &tilemap->tex);
}

inline void
hs_aroom_set_xy(hs_aroom* aroom, const uint16_t x, const uint16_t y, const uint16_t data)
{
        aroom->data[x + y * aroom->width] = data;
}

void
hs_aroom_to_tilemap(const hs_aroom aroom, hs_tilemap* tilemap, const uint16_t layer)
{
        tilemap->width = aroom.width;
        tilemap->height = aroom.height;
        hs_tilemap_init(tilemap, 0);

        hs_aroom_set_tilemap(aroom, tilemap, layer);
}

inline uint8_t
hs_aroom_get_xy(const hs_aroom aroom, const uint16_t x, const uint16_t y)
{
        return aroom.data[x + y * aroom.width];
}

void
hs_aroom_set_tilemap(const hs_aroom aroom, hs_tilemap* tilemap, const uint16_t layer)
{
        assert(tilemap->width  == aroom.width);
        assert(tilemap->height == aroom.height);

        if (layer > 1) {
                const uint32_t offset = (aroom.width * aroom.height * layer) - (aroom.width * aroom.height);
                for(uint32_t i = 0; i < aroom.width * aroom.height; i++)
                        hs_tilemap_set(tilemap, i, aroom.data[i + offset]);
        } else {
                for(uint32_t i = 0; i < aroom.width * aroom.height; i++)
                        hs_tilemap_set(tilemap, i, aroom.data[i]);
        }
        hs_tilemap_update_vbo(*tilemap);
}

void
hs_aroom_set_tilemap_offsetv(const hs_aroom aroom, hs_tilemap* tilemap, const uint16_t layer, const vec2i offset)
{
        assert(tilemap->width  >= offset.x + aroom.width);
        assert(tilemap->height >= offset.y + aroom.height);

        if (layer > 1) {
                const uint32_t layer_offset = (aroom.width * aroom.height * layer) - (aroom.width * aroom.height);
                uint32_t i = 0;
                for(uint32_t x = offset.x; x < aroom.width; x++)
                        for(uint32_t y = offset.y; y < aroom.height; y++)
                                hs_tilemap_set_xy(tilemap, x, y, aroom.data[layer_offset + i++]);
        } else {
                uint32_t i = 0;
                for(uint32_t x = offset.x; x < offset.x + aroom.width; x++)
                        for(uint32_t y = offset.y; y < offset.y + aroom.height; y++)
                                hs_tilemap_set_xy(tilemap, x, y, aroom.data[i++]);
        }
}

vec2
hs_tilemap_pos_to_global(const hs_tilemap tilemap, vec2i pos)
{
        return (vec2) {
                .x = (float)pos.x * 2.0f * tilemap.tile_width  - (float)tilemap.width  * tilemap.tile_width,
                .y = (float)pos.y * 2.0f * tilemap.tile_height - (float)tilemap.height * tilemap.tile_height,
        };
}

uint32_t
hs_dyn_tilemap_sizeof(const hs_dyn_tilemap tilemap)
{
        return sizeof(hs_tex_square) * tilemap.vertices.len;
}

void
hs_dyn_tilemap_init(hs_dyn_tilemap* tilemap, size_t size)
{
        if (tilemap->tile_height    <= 0.0f) tilemap->tile_height = 0.1f;
        if (tilemap->tile_width     <= 0.0f) tilemap->tile_width = 0.1f;
        if (tilemap->tileset_height == 0)    tilemap->tileset_height = 1;
        if (tilemap->tileset_width  == 0)    tilemap->tileset_width = 1;
        if (tilemap->tex            == 0)    tilemap->tex = hs_default_missing_tex;
        if (size                    == 0)    size = 10000;

        tilemap->vertices = hs_dynarr_init(hs_tex_square, size);

        hs_vobj vobj = hs_vobj_create(castf(tilemap->vertices.data), hs_dyn_tilemap_sizeof(*tilemap), 0, 0, GL_DYNAMIC_DRAW, 1);
        tilemap->sp = hs_shader_program_create(hs_sp_texture_transform_create(), vobj);

        hs_tex_uniform_set(hs_uniform_create(tilemap->sp.p, "u_tex"), 0);
        tilemap->sp.coord = hs_uniform_coord_create(tilemap->sp.p, "u_model", "u_view", "u_proj");
        hs_uniform_mat4_set(tilemap->sp.coord.proj, (mat4)MAT4_IDENTITY);

        hs_vattrib_enable_float(0, 2, 4, 0);
        hs_vattrib_enable_float(1, 2, 4, 2);
}

inline void
hs_dyn_tilemap_clear(hs_dyn_tilemap* tilemap)
{
        hs_dynarr_clear(tilemap->vertices);
}

inline void
hs_dyn_tilemap_free(hs_dyn_tilemap* tilemap)
{
        hs_dynarr_free(tilemap->vertices);
        hs_sp_delete(tilemap->sp);
        glDeleteTextures(1, &tilemap->tex);
}

void
hs_dyn_tilemap_push(hs_dyn_tilemap* tilemap, const vec2i pos, uint32_t tile)
{
        hs_tex_square new_tile;

        // make tiles start at 0 instead of 1
        tile++;

        const float width      = 1.0f/tilemap->tileset_width;
        const float height     = 1.0f/tilemap->tileset_height;
        const float ypos       = height * ceilf((float)tile / (float)tilemap->tileset_width);
        float xpos             = width * (tile % tilemap->tileset_width);
        if (xpos == 0.0f) xpos = 1.0f;

        hs_aabb2 tex = {
                .bl = (vec2){xpos - width, ypos - height},
                .tr = (vec2){xpos, ypos},
        };
        hs_tex_square_set_tex(&new_tile, tex);

        vec2 offset = hs_dyn_tilemap_pos_to_global(*tilemap, pos);
        hs_aabb2 square_pos;
        square_pos.bl.x = offset.x - tilemap->tile_width;
        square_pos.bl.y = offset.y - tilemap->tile_height;
        square_pos.tr.x = offset.x + tilemap->tile_width;
        square_pos.tr.y = offset.y + tilemap->tile_height;
        hs_tex_square_set_pos(&new_tile, square_pos);

        hs_dynarr_push(tilemap->vertices, hs_tex_square, new_tile);
}

void
hs_dyn_tilemap_update_vbo(const hs_dyn_tilemap tilemap)
{
        glBindBuffer(GL_ARRAY_BUFFER, tilemap.sp.vobj.vbo);
        glBufferData(GL_ARRAY_BUFFER, hs_dyn_tilemap_sizeof(tilemap), castf(tilemap.vertices.data), GL_DYNAMIC_DRAW);
}

inline void
hs_dyn_tilemap_draw(const hs_dyn_tilemap tilemap)
{
        hs_sp_use(tilemap.sp);
        glDrawArrays(GL_TRIANGLES, 0, 6 * tilemap.vertices.len);
}

void
hs_aroom_push_dyn_tilemap(const hs_aroom aroom, hs_dyn_tilemap* tilemap, const uint16_t layer, const vec2i offset)
{
        vec2i pos;
        if (layer > 1) {
                const uint32_t layer_offset = (aroom.width * aroom.height * layer) - (aroom.width * aroom.height);
                uint32_t i = 0;
                for(uint32_t x = offset.x; x < aroom.width; x++)
                        for(uint32_t y = offset.y; y < aroom.height; y++)
                                hs_dyn_tilemap_push(tilemap, (vec2i){x + offset.x, y + offset.y},
                                                    aroom.data[layer_offset + i++]);
        } else {
                uint32_t i = 0;
                for(uint32_t x = 0; x < aroom.width; x++)
                        for(uint32_t y = 0; y < aroom.height; y++)
                                hs_dyn_tilemap_push(tilemap, (vec2i){x + offset.x, y + offset.y},
                                                    aroom.data[i++]);
        }
}

vec2
hs_dyn_tilemap_pos_to_global(const hs_dyn_tilemap tilemap, vec2i pos)
{
        return (vec2) {
                .x = (float)pos.x * 2.0f * tilemap.tile_width,
                .y = (float)pos.y * 2.0f * tilemap.tile_height,
        };
}

inline hs_shader_program
hs_sp_sprite_create(const float width, const float height, const float screen_size)
{
        const float vertices[] = HS_DEFAULT_SQUARE_SCALED_TEX_VERT_ONLY(
                width/screen_size, width/screen_size);

        hs_vobj vobj = hs_vobj_create(vertices, sizeof(vertices), 0, 0, GL_STATIC_DRAW, 1);
        hs_shader_program sp = hs_shader_program_create(hs_sp_texture_transform_create(), vobj);
        sp.coord = hs_uniform_coord_create(sp.p, "u_model", "u_view", "u_proj");
        hs_uniform_mat4_set(sp.coord.proj, (mat4)MAT4_IDENTITY);

        hs_vattrib_enable_float(0, 2, 4, 0);
        hs_vattrib_enable_float(1, 2, 4, 2);

        return sp;
}

inline void
hs_sprite_draw_current()
{
        glDrawArrays(GL_TRIANGLES, 0, 6);
}

inline hs_entity2
hs_entity2_create(hs_entity2_hot* hot, hs_shader_program sp, hs_tex tex)
{
        hot->sp = sp;
        hot->tex = tex;
        return (hs_entity2){.hot = hot};
}

inline uint32_t
hs_vao_create(const uint32_t count)
{
        uint32_t vao;
        glGenVertexArrays(count, &vao);
        glBindVertexArray(vao);
        return vao;
}

inline uint32_t
hs_vbo_create(const float *vbuff, const uint32_t buffsize, const GLenum usage, const uint32_t count)
{
        uint32_t vbo;
        glGenBuffers(count, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, buffsize, vbuff, usage);
        return vbo;
}

inline uint32_t
hs_ebo_create(const uint32_t *ibuff, const uint32_t buffsize, const GLenum usage, const uint32_t count)
{
        if(!buffsize) return 0;
        uint32_t ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffsize, ibuff, usage);
        return ebo;
}

inline hs_vobj
hs_vobj_create(const float    *vbuff, const uint32_t vbuffsize,
               const uint32_t *ibuff, const uint32_t ibuffsize,
               const GLenum    usage, const uint32_t count)
{
        hs_vobj vobj;
        vobj.vbo = hs_vbo_create(vbuff, vbuffsize, usage, count);
        vobj.ebo = hs_ebo_create(ibuff, ibuffsize, usage, count);
        vobj.vao = hs_vao_create(count);
        vobj.count = count;
        return vobj;
}

inline void
hs_vobj_free(hs_vobj vobj)
{
        glDeleteVertexArrays(vobj.count, &vobj.vao);
        if (vobj.ebo)
                glDeleteBuffers(vobj.count, &vobj.ebo);
        glDeleteBuffers(vobj.count, &vobj.vbo);
}

inline void
hs_fps_callback_init(const hs_game_data gd, void(*mouse_callback)(GLFWwindow*, double xpos, double ypos))
{
        glfwSetInputMode(gd.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(gd.window, mouse_callback);
}

inline static void
hs_disable_vsync()
{
        glfwSwapInterval(0);
}

enum hs_init_flags {
        HS_NO_VSYNC = 1 << 0,
        HS_WIREFRAME_MODE = 1 << 1,
        HS_BLEND_MODE = 1 << 2,
        HS_DEPTH_TESTING = 1 << 3,
};

inline static void
hs_init(hs_game_data* gd, const char *name, void(*framebuffer_size_callback)(GLFWwindow*, int, int), const uint32_t flags)
{
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if (!gd->width) gd->width = 800;
        if (!gd->height) gd->height = 600;

        GLFWwindow* window = glfwCreateWindow(gd->width, gd->height, name, NULL, NULL);
        assert(window);

        glfwMakeContextCurrent(window);
        assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
        glViewport(0, 0, gd->width, gd->height);

#ifndef NO_STBI
        {
                glGenTextures(1, &hs_default_missing_tex);
                glBindTexture(GL_TEXTURE_2D, hs_default_missing_tex);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, hs_default_missing_tex_data);
                glGenerateMipmap(GL_TEXTURE_2D);
        }
#endif // NO_STBI

        if (flags & HS_WIREFRAME_MODE) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (flags & HS_NO_VSYNC) hs_disable_vsync();
        if (flags & HS_DEPTH_TESTING) glEnable(GL_DEPTH_TEST);

        if (flags & HS_BLEND_MODE) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        if (framebuffer_size_callback) {
                glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
                int width, height;
                glfwGetFramebufferSize(window, &width, &height);
                framebuffer_size_callback(window, width, height);
        }

        gd->window = window;
}

inline static void
hs_poll_input()
{
        glfwPollEvents();
}

inline static void
hs_end_frame(const hs_game_data gd)
{
        glfwSwapBuffers(gd.window);
}

inline static void
hs_exit() {glfwTerminate();}

#endif // HS_IMPL

#endif // HS_GRAPHICS_H_
