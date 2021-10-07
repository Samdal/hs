#ifndef HS_MATH_H_
#define HS_MATH_H_

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef NO_STDIO
#include <stdio.h>
#endif

#define sq(_x) (((_x)*(_x)))
#define castf(_val) ((float*)(_val))

#define PI 3.141592653589793
#define HALF_PI 1.570796326794897
#define TWO_PI 6.283185307179586
#define EULER 2.718281828459045

#define deg2rad(deg) ((deg * PI) / 180.0)

#define MAT4_DIAG(__f)                          \
        {{__f, 0.f, 0.f, 0.f},                  \
         {0.f, __f, 0.f, 0.f},                  \
         {0.f, 0.f, __f, 0.f},                  \
         {0.f, 0.f, 0.f, __f}}
#define MAT4_IDENTITY MAT4_DIAG(1.0f)
#define VEC3_SAME(__f) (vec3){__f, __f, __f}

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

// a is min, b is max
#define constrain(x, a, b) ({x = x < a ? a : x; x = x > b ? b : x;})


typedef union {
        struct {
                float x, y;
        };
        float axis[2];
        float xy[2];
} vec2;

typedef union {
        struct {
                int32_t x, y;
        };
        int32_t axis[2];
        int32_t xy[2];
} vec2i;

typedef union {
        struct {
                float x, y, z;
        };
        struct {
                float r, g, b;
        };
        float xyz[3];
        float axis[4];
        float rgb[3];
} vec3;

typedef union {
        struct {
                float x, y, z, w;
        };
        struct {
                float r, g, b, a;
        };
        float xyzw[4];
        float axis[4];
        float rgba[4];
} vec4;

typedef float mat2[2][2];
typedef float mat3[3][3];
typedef float mat4[4][4];

inline static float
random_float()
{
        // double for better rounding
        return (float)((double)rand()/RAND_MAX);
}

inline static float
random_float_negative()
{
        // double for better rounding
        return (float)((double)rand()/(RAND_MAX) * 2.0 -1.0);
}

inline static float
vec2_len(vec2 vector)
{
        return sqrtf(sq(vector.x) + sq(vector.y));
}

inline static vec2
vec2_add(const vec2 v1, const vec2 v2)
{
        return (vec2){v1.x + v2.x, v1.y + v2.y};
}

inline static void
vec2_add_in_place(vec2* v1, const vec2 v2)
{
        v1->x += v2.x;
        v1->y += v2.y;
}

inline static vec2
vec2_sub(const vec2 v1, const vec2 v2)
{
        return (vec2){v1.x - v2.x, v1.y - v2.y};
}

inline static float
vec2_dot(const vec2 v1, const vec2 v2)
{
        return v1.x * v2.x + v1.y * v2.y;
}

inline static float
vec2_cos(const vec2 v1, const vec2 v2)
{
        return vec2_dot(v1, v2) / (vec2_len(v1) * vec2_len(v2));
}

inline static vec2
vec2_mul(const vec2 v1, const vec2 v2)
{
        return (vec2){
                v1.x * v2.x,
                v1.y * v2.y,
        };
}

inline static vec2
vec2_scale(const vec2 vector, const float scalar)
{
        return (vec2){
                vector.x * scalar,
                vector.y * scalar,
        };
}

inline static vec2i
vec2i_scale(const vec2i vector, const int32_t scalar)
{
        return (vec2i){
                vector.x * scalar,
                vector.y * scalar,
        };
}

inline static vec2
vec2_norm(const vec2 vector)
{
        float len = vec2_len(vector);
        return len == 0.0f ? vector : vec2_scale(vector, 1.0f / len);
}

inline static uint32_t
vec2_null(const vec2 vector)
{
        return (vector.x == 0 && vector.y == 0);
}

inline static vec2i
vec2i_div(const vec2i vector, const int32_t divisor)
{
        return (vec2i){
                vector.x / divisor,
                vector.y / divisor,
        };
}

inline static vec2i
vec2i_add(const vec2i v1, const vec2i v2)
{
        return (vec2i){v1.x + v2.x, v1.y + v2.y};
}

inline static vec2i
vec2i_sub(const vec2i v1, const vec2i v2)
{
        return (vec2i){v1.x - v2.x, v1.y - v2.y};
}

inline static float
vec3_len(const vec3 vector)
{
        return sqrtf(sq(vector.x) + sq(vector.y) + sq(vector.z));
}

inline static vec3
vec3_scale(const vec3 vector, const float scalar)
{
        return (vec3){
                vector.x * scalar,
                vector.y * scalar,
                vector.z * scalar,
        };
}

inline static vec3
vec3_sub(const vec3 v1, const vec3 v2)
{
        return (vec3){
                v1.x - v2.x,
                v1.y - v2.y,
                v1.z - v2.z
        };
}

inline static vec3
vec3_add(const vec3 v1, const vec3 v2)
{
        return (vec3){
                v1.x + v2.x,
                v1.y + v2.y,
                v1.z + v2.z
        };
}

inline static vec3
vec3_mul(const vec3 v1, const vec3 v2)
{
        return (vec3){
                v1.x * v2.x,
                v1.y * v2.y,
                v1.z * v2.z
        };
}

inline static vec3
vec3_cross(const vec3 v1, const vec3 v2)
{
        return (vec3){
                v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x
        };
}

inline static vec3
vec3_norm(const vec3 vector)
{
        float len = vec3_len(vector);
        return len == 0.0f ? vector : vec3_scale(vector, 1.0f / len);
}

inline static float
vec3_dot(const vec3 v1, const vec3 v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

inline static vec4
vec4_scale(const vec4 vector, const float scalar)
{
        return (vec4){
                vector.x * scalar,
                vector.y * scalar,
                vector.z * scalar,
                vector.w * scalar,
        };
}

#ifndef NO_STDIO
inline static void
mat4_print(const mat4 mat)
{
        for (uint32_t i = 0; i < 4; i++)
                printf("mat[%d] is %f %f %f %f\n", i, mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
}
inline static void
vec3_print(const vec3 vec)
{
        printf("vec3 is %f %f %f\n", vec.x, vec.y, vec.z);
}
#endif

inline static void
mat4_mul(mat4 res, const mat4 m1, const mat4 m2)
{
        memset(res, 0, sizeof(mat4));
        for (uint32_t y = 0; y < 4; y++)
                for (uint32_t x = 0; x < 4; x++)
                        for (uint32_t e = 0; e < 4; e++)
                                res[x][y] += m1[x][e] * m2[e][y];
}

inline static void
mat4_scale(mat4 mat, const vec3 vector)
{
        for (unsigned i = 0; i < 3; i++)
                mat[i][i] *= vector.xyz[i];
}

inline static void
mat4_translate(mat4 mat, const vec3 vector)
{
        for (unsigned i = 0; i < 3; i++) {
                mat[3][i] += vector.xyz[i];
        }
}

inline static void
mat4_rotate(mat4 mat, const float angle, const vec3 axis)
{
        mat4 matr = MAT4_IDENTITY;

        float c = cosf(angle);
        float s = sinf(angle);

        vec3 naxis = vec3_norm(axis);
        float x = naxis.x;
        float y = naxis.y;
        float z = naxis.z;

        matr[0][0] = x * x * (1 - c) + c;
        matr[0][1] = x * y * (1 - c) + z * s;
        matr[0][2] = x * z * (1 - c) - y * s;

        matr[1][0] = x * y * (1 - c) - z * s;
        matr[1][1] = y * y * (1 - c) + c;
        matr[1][2] = y * z * (1 - c) + x * s;

        matr[2][0] = x * z * (1 - c) + y * s;
        matr[2][1] = y * z * (1 - c) - x * s;
        matr[2][2] = z * z * (1 - c) + c;

        mat4 m2;
        memcpy(m2, mat, sizeof(m2));
        mat4_mul(mat, m2, matr);
}

inline static void
mat4_ortho(mat4 res, const float l, const float r, const float b, const float t, const float n, const float f)
{
        // Main diagonal
        mat4_scale(res, (vec3){2.0f / (r - l), 2.0f / ( t - b ), -2.0f / (f - n)});
        return;
        // Last column
        mat4_translate(res, (vec3){-(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n)});
}

// Expects an empty identity mat4
inline static void
mat4_perspective(mat4 res, const float fov, const float asp_ratio, const float n, const float f)
{
        float q = 1.0f / tanf(0.5f * fov);

        res[0][0] = q / asp_ratio;
        res[1][1] = q;
        res[2][2] = (n + f) / (n - f);
        res[3][2] = (2.0f * n * f) / (n - f);
        res[2][3] = -1.0f;
}

inline static void
mat4_look_at(mat4 res, const vec3 position, const vec3 target, const vec3 up)
{
    vec3 f = vec3_norm(vec3_sub(target, position));
    vec3 s = vec3_norm(vec3_cross(f, up));
    vec3 u = vec3_cross(s, f);

    res[0][0] = s.x;
    res[1][0] = s.y;
    res[2][0] = s.z;

    res[0][1] = u.x;
    res[1][1] = u.y;
    res[2][1] = u.z;

    res[0][2] = -f.x;
    res[1][2] = -f.y;
    res[2][2] = -f.z;

    res[3][0] = -vec3_dot(s, position);
    res[3][1] = -vec3_dot(u, position);
    res[3][2] = vec3_dot(f, position) + 1.0f;
}

/* inline static vec4 */
/* mat4_mul_vec4(const mat4 mat, const vec4 vec) */
/* { */
/*         vec4 res; */

/*         for (unsigned i = 0; i < 3; i++) */
/*                 res.xyzw[i] = mat[i][i] * vec.xyzw[i] + mat[3][i]; */
/*         res.w = mat[3][3] * vec.w; */

/*         return res; */
/* } */

/* inline static vec4 */
/* mat4_mul_vec3(const mat4 mat, const vec3 vec) */
/* { */
/*         vec4 v = (vec4){vec.x, vec.y, vec.z, 1.0f}; */
/*         return mat4_mul_vec4(mat, v); */
/* } */

#endif // HS_MATH_H_
