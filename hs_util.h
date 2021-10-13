#ifndef HS_UTIL_H_
#define HS_UTIL_H_

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct {
        size_t cap, len;
        void* data;
} hs_dynarr;

#define hs_dynarr_init(_type, _cap) _hs_dynarr_init_sz(sizeof(_type), _cap)
#define hs_dynarr_zero(_arr, _type) memset(_arr.data, 0, sizeof(_type) * _arr.cap)
#define hs_dynarr_data(_arr, _type) ((_type*)_arr.data)
#define hs_dynarr_idx(_arr, _type, _index) hs_dynarr_data(_arr, _type)[_index]

#define hs_dynarr_resize(_arr, _type, _size) _hs_dynarr_resize(&_arr, sizeof(_type), _size)
#define hs_dynarr_clear(_arr) _arr.len = 0
#define hs_dynarr_free(_arr)                    \
        free(_arr.data);                        \
        _arr = (hs_dynarr){0}

#define hs_dynarr_pop(_arr) if(_arr.len > 0) _arr.len--
#define hs_dynarr_push(_arr, _type, _value)                             \
        if (++_arr.len >= _arr.cap)                                     \
                _hs_dynarr_resize(&_arr, sizeof(_type), _arr.cap * 2);  \
        hs_dynarr_idx(_arr, _type, _arr.len - 1) = _value

#define hs_dynarr_front(_arr, _type) hs_dynarr_idx(_arr, _type, _arr.len - 1)
#define hs_dynarr_back(_arr, _type) hs_dynarr_idx(_arr, _type, 0)

static inline hs_dynarr
_hs_dynarr_init_sz(size_t type_size, const size_t capacity)
{
        void* data = malloc(type_size * capacity);
        assert(data);

        return (hs_dynarr){
                .cap  = capacity,
                .data = data,
        };
}

static inline void
_hs_dynarr_resize(hs_dynarr* dynarr, const size_t type_size, const size_t new_cap)
{
        if (new_cap > dynarr->cap) {
                void* new_data = malloc(type_size * new_cap);
                assert(new_data);

                memcpy(new_data, dynarr->data, dynarr->cap * type_size);
                free(dynarr->data);
                dynarr->data = new_data;
        }
        dynarr->cap = new_cap;
}

extern void hs_memsetv(void* restrict dst, const size_t num, void* restrict src, const size_t sz);

extern char*    hs_file_read_null_term(const char *file_path);
extern uint8_t* hs_file_read(const char *file_path);

#ifdef HS_IMPL
#define HS_UTIL_IMPL
#endif // HS_IMPL

#ifdef HS_UTIL_IMPL

void
hs_memsetv(void* restrict dst, const size_t num, void* restrict src, const size_t sz)
{
        const size_t dst_end = (size_t)dst + num * sz;
        for(; (size_t)dst < dst_end; dst += sz)
                memcpy(dst, src, sz);
}

char*
hs_file_read_null_term(const char *file_path)
{
        FILE *file = fopen(file_path, "r");
        if (!file) {
                fprintf(stderr, "---error reading file \"%s\"---\n", file_path);
                assert(file);
        }

        fseek(file, 0L, SEEK_END);
        uint32_t readsize = ftell(file);
        rewind(file);

        char* buffer = malloc(readsize + 1);
        assert(buffer);

        fread(buffer, 1, readsize, file);
        buffer[readsize] = '\0';

        fclose(file);
        return buffer;
}

uint8_t*
hs_file_read_(const char *file_path)
{
        FILE *file = fopen(file_path, "r");
        if (!file) {
                fprintf(stderr, "---error reading file \"%s\"---\n", file_path);
                assert(file);
        }

        fseek(file, 0L, SEEK_END);
        uint32_t readsize = ftell(file);
        rewind(file);

        uint8_t* buffer = malloc(readsize);
        assert(buffer);

        fread(buffer, 1, readsize, file);

        fclose(file);
        return buffer;
}

#undef HS_UTIL_IMPL
#endif // HS_UTIL_IMPL

#endif // HS_UTIL_H_
