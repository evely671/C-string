#include "cstr.h"
#include <string.h>

int cstr_init(cstr* dst) {
	if (!dst) 
        return CSTR_INVARG;

    char* new_str = malloc(1);
    if (!new_str)
        return CSTR_NOMEN;

    dst->str = new_str;
    dst->str[0] = '\0';
    dst->byte_len = 0;
    dst->cap = 1;
    return CSTR_OK;
}

int cstr_clear(cstr* dst) {
    if (!dst)
        return CSTR_INVARG;

    free(dst->str);

    dst->str = NULL;
    dst->byte_len = 0;
    dst->cap = 0;
    return CSTR_OK;
}

cstr* cstr_create_p(void) {
    cstr* s = malloc(sizeof(cstr));

    if (!s)
        return NULL;
    if (cstr_init(s) != CSTR_OK) {
        free(s);
        return NULL;
    }
    return s;
}

int cstr_dstr_p(cstr* dst) {
    if (!dst)
        return CSTR_INVARG;
    
    free(dst->str);
    free(dst);
    return CSTR_OK;
}

int cstr_reset(cstr* dst) {
    if (!dst)
        return CSTR_INVARG;

    if (dst->byte_len > 0) {
        dst->byte_len = 0;

        if (dst->str)
            dst->str[0] = '\0';
    }
    return CSTR_OK;
}

int cstr_set(cstr* dst, const char* src) {
    if (!dst || !src)
        return CSTR_INVARG;

    size_t new_byte_len = strlen(src);
    
    if (dst->cap < new_byte_len) {
        size_t new_cap = new_byte_len * 2 + 1;
        char* new_str = realloc(dst->str, new_cap);

        if (!new_str)
            return CSTR_NOMEN;

        dst->str = new_str;
        dst->cap = new_cap;
    }

    strcpy(dst->str, src);
    
    dst->byte_len = new_byte_len;
    return CSTR_OK;
}

int cstr_remove(cstr* dst, size_t pos, size_t byte_len) {
    if (!dst || pos > dst->byte_len || byte_len == 0)
        return CSTR_INVARG;

    if (byte_len > dst->byte_len - pos)
        byte_len = dst->byte_len - pos;

    char* new_str = dst->str;

    memmove(new_str + pos,
            new_str + pos + byte_len,
            dst->byte_len - pos - byte_len);

    dst->byte_len -= byte_len;
    new_str[dst->byte_len] = '\0';
    return CSTR_OK;
}

int cstr_setcap(cstr* dst, size_t cap) {
    if (!dst) return CSTR_INVARG;

    size_t min_req = dst->byte_len + 1;
    if (cap < min_req) cap = min_req;

    if (cap == dst->cap) return CSTR_OK;

    char* new_str = realloc(dst->str, cap);
    if (!new_str) return CSTR_NOMEN;

    dst->str = new_str;
    dst->cap = cap;
    return CSTR_OK;
}

int cstr_setcap_unsafe(cstr* dst, size_t cap) {
    if (!dst)
        return CSTR_INVARG;
    
    char* new_str = realloc(dst->str, cap);
    
    if (!new_str)
        return CSTR_NOMEN;
    
    dst->str = new_str;
    dst->cap = cap;
    return CSTR_OK;
}

int cstr_shrink(cstr* dst) {
    if (!dst)
        return CSTR_INVARG;
    return cstr_setcap(dst, dst->byte_len + 1);
}

int cstr_insert(cstr* dst, const char* src, size_t pos) {
    if (!dst || !src || pos > dst->byte_len)
        return CSTR_INVARG;

    size_t src_len = strlen(src);
    size_t new_len = src_len + dst->byte_len;
    
    if (new_len < strlen(src))
        return CSTR_NOMEN;

    if (new_len + 1 > dst->cap) {
        size_t new_cap = dst->cap ? dst->cap * 2 : 32; /* if cap is 0, set it to 32 | 如果 cap 为 0, 则设置它为32 */
        
        while (new_cap < new_len + 1)
            new_cap *= 2;

        char* new_str = realloc(dst->str, new_cap);
        
        if (!new_str)
            return CSTR_NOMEN;

        dst->str = new_str;
        dst->cap = new_cap;
    }

    if (dst->byte_len > 0 && pos < dst->byte_len) {
        memmove(dst->str + pos + src_len,
            dst->str + pos,
            dst->byte_len - pos);
    }

    memcpy(dst->str + pos, src, src_len);
    dst->byte_len = new_len;
    dst->str[new_len] = '\0';

    return CSTR_OK;
}

int cstr_append(cstr* dst, const char* src) {
    return cstr_insert(dst, src, dst->byte_len);
}
