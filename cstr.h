#ifndef SCSTR_H
#define SCSTR_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CSTR_OK     0
#define CSTR_NOMEN  1
#define CSTR_INVARG 2

	typedef struct scstr {
		char* str;
		size_t byte_len;
		size_t cap;
	}cstr;

#define CSTR_INIT  {NULL, 0, 0}

	int cstr_init(cstr* dst);
	int cstr_clear(cstr* dst);
	
	cstr* cstr_create_p(void);
	int cstr_dstr_p(cstr* dst);

	int cstr_reset(cstr* dst);
	int cstr_set(cstr* dst, const char* src);
	int cstr_remove(cstr* dst, size_t pos, size_t len);

	int cstr_setcap(cstr* dst, size_t cap);
	int cstr_setcap_unsafe(cstr* dst, size_t cap);
	int cstr_shrink(cstr* dst);

	int cstr_insert(cstr* dst, const char* src, size_t pos);
	int cstr_append(cstr* dst, const char* src);

#ifdef __cplusplus
}
#endif
#endif