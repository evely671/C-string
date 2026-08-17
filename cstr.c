#include "cstr.h"

struct cs {
    char* str;
    size_t byte_len;
    size_t cap;
};

static size_t cs_strlen(const char* s) {
    if (!s)
        return 0;

    size_t len = 0;

    while (s[len] != '\0') {
        len++;
    }
    return len;
}

static int cs_copy(cs* dst, const char* src) { /* Return the length instead of the macro definition. | 返回长度而不是宏定义 */
    if (!dst || !src || dst->cap == 0)
        return 0;

    size_t i = 0;

    while (i < dst->cap - 1 && src[i] != '\0') {
        dst->str[i] = src[i];
        i++;
    }

    dst->str[i] = '\0';
    return i;
}


int cs_init(cs* dst) {
    return cs_init_with(dst, "", 16);
}

int cs_init_with(cs* dst, const char* src, size_t cap) {
    if (!dst)
        return CS_INVARG;

    *dst = (cs){ 0 };

    size_t src_len = src ? cs_strlen(src) : 0; /* if src equals 'NULL', set src to 0, otherwise, set it to cs_strlen(src) | 如果 src 等于 'NULL'，就把 src 设为 0，否则就把它设为 cs_strlen(src)*/
    size_t min_cap = src_len + 1;

    if (cap < min_cap)
        cap = min_cap;

    if (cap == 0) {
        return CS_OK;
    }

    dst->str = (char*)malloc(cap);

    if (!dst->str)
        return CS_NOMEN;

    dst->cap = cap;

    if (src) {
        dst->byte_len = cs_copy(dst, src);
    }
    else {
        dst->byte_len = 0;
        dst->str[0] = '\0';
    }

    return CS_OK;
}

int cs_init_empty(cs* dst) {
    return cs_init_with(dst, NULL, 0);
}

void cs_dstr(cs* dst) {
    if (!dst)
        return;

    free(dst->str);
    dst->str = NULL;
    dst->byte_len = 0;
    dst->cap = 0;
}

int cs_setcap(cs* dst, size_t cap) {
    if (!dst) return CS_INVARG;

    size_t min_req = dst->byte_len + 1;
    if (cap < min_req) cap = min_req;

    if (cap == dst->cap) return CS_OK;

    char* new_str = realloc(dst->str, cap);
    if (!new_str) return CS_NOMEN;

    dst->str = new_str;
    dst->cap = cap;
    return CS_OK;
}

int cs_setcap_unsafe(cs* dst, size_t cap) { /* Actually, the function will automatically make sure the string is a normal C string. | 实际上，函数会自动确保字符串为正常的C字符串 */
    if (!dst)
        return CS_INVARG;

    char* new_str = realloc(dst->str, cap + 1);

    if (!new_str)
        return CS_NOMEN;

    new_str[cap] = '\0';
    dst->str = new_str;
    dst->cap = cap + 1;
    return CS_OK;
}

int cs_shrink(cs* dst) {
    if (!dst)
        return CS_INVARG;
    return cs_setcap(dst, dst->byte_len + 1);
}

size_t cs_cap(const cs* dst) {
    if (!dst)
        return CS_INVARG;
    return dst->cap;
}

int cs_set(cs* dst, const char* src) {
    if (!dst || !src)
        return CS_INVARG;

    size_t new_byte_len = strlen(src);
    size_t new_cap;

    if (dst->cap < new_byte_len + 1) {
        if (new_byte_len <= 256)
            new_cap = new_byte_len + 16;
        else
            new_cap = new_byte_len + new_byte_len / 2;

        char* new_str = realloc(dst->str, new_cap);

        if (!new_str)
            return CS_NOMEN;

        dst->str = new_str;
        dst->cap = new_cap;
    }

    cs_copy(dst, src);

    dst->byte_len = new_byte_len;
    return CS_OK;
}

int cs_clear_length(cs* dst) {
    if (!dst)
        return CS_INVARG;

    dst->byte_len = 0;
    return CS_OK;
}

int cs_append(cs* dst, const char* src) {
    return cs_insert(dst, src, dst->byte_len);
}

int cs_insert(cs* dst, const char* src, size_t pos) {
    if (!dst || !src || pos > dst->byte_len)
        return CS_INVARG;

    size_t src_len = strlen(src);
    size_t new_len = src_len + dst->byte_len;

    if (new_len < strlen(src))
        return CS_NOMEN;

    if (new_len + 1 > dst->cap) {
        size_t new_cap;

        if (dst->cap == 0)
            new_cap = 16;
        else
            new_cap = dst->cap + dst->cap / 2;

        if (new_cap < new_len + 1)
            if (new_len <= 256)
                new_cap += 16;
            else
                new_cap += new_cap / 2;

        char* new_str = realloc(dst->str, new_cap);

        if (!new_str)
            return CS_NOMEN;

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

    return CS_OK;
}

int cs_remove(cs* dst, size_t pos, size_t byte_len) {
    if (!dst || pos > dst->byte_len || byte_len == 0)
        return CS_INVARG;

    if (byte_len > dst->byte_len - pos)
        byte_len = dst->byte_len - pos;

    char* new_str = dst->str;

    memmove(new_str + pos,
        new_str + pos + byte_len,
        dst->byte_len - pos - byte_len);

    dst->byte_len -= byte_len;
    new_str[dst->byte_len] = '\0';
    return CS_OK;
}

int cs_reset(cs* dst) {
    if (!dst)
        return CS_INVARG;

    if (dst->byte_len > 0) {
        dst->byte_len = 0;

        if (dst->str)
            dst->str[0] = '\0';
    }

    return CS_OK;
}

int cs_find(cs* dst, const char* src) {
    if (!dst || !src)
        return CS_INVARG;

    if (src[0] == '\0')
        return 0;

    if (dst->str == NULL)
        return -1;

    char* found = strstr(dst->str, src);
    if (found == NULL)
        return -1;

    size_t pos = found - dst->str;

    if (pos > (size_t)INT_MAX)
        return -1;

    return (int)pos;
}

int cs_rfind(cs* dst, const char* src) {
    if (!dst || !src)
        return CS_INVARG;

    if (src[0] == '\0') {
        if (dst->byte_len > (size_t)INT_MAX)
            return -1;
        return (int)dst->byte_len;
    }

    if (!dst->str || dst->byte_len == 0)
        return -1;

    size_t src_len = strlen(src);
    if (src_len > dst->byte_len)
        return -1;

    for (size_t i = dst->byte_len - src_len; i != (size_t)-1; --i) {
        if (memcmp(dst->str + i, src, src_len) == 0) {
            return (int)i;
        }
    }

    return -1;
}

int cs_equal_len(const cs* a, const cs* b) {
    if (!a || !b)
        return 1;
    return a->byte_len != b->byte_len;
}

int cs_cmp_str(const cs* a, const cs* b) {
    if (cs_equal_len(a, b) != 0)
        return 1;

    if (a->byte_len == 0)
        return 0;

    return memcmp(a->str, b->str, a->byte_len) != 0;
}

int cs_cmp_cstr(const cs* a, const char* b) {
    if (!a || !b) return 1;

    size_t blen = strlen(b);
    if (a->byte_len != blen)
        return 1;

    if (a->byte_len == 0)
        return 0;

    return strcmp(a->str, b) != 0;
}

const char* cs_data(const cs* dst) {
    if (!dst)
        return NULL;
    return dst->str;
}

size_t cs_length(const cs* dst) {
    if (!dst)
        return CS_INVARG;
    return dst->byte_len;
}
