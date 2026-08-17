#ifndef CSTR_H
#define CSTR_H

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CS_OK     0
#define CS_NOMEN  1
#define CS_INVARG 2

	typedef struct cs cs;

#define CSTR_INIT  {NULL, 0, 0}

	int cs_init(cs* dst);
	int cs_init_with(cs* dst, const char* src, size_t cap);
	int cs_init_empty(cs* dst);
	void cs_dstr(cs* dst);

	int cs_setcap(cs* dst, size_t cap);
	int cs_setcap_unsafe(cs* dst, size_t cap);
	int cs_shrink(cs* dst);

	size_t cs_cap(const cs* dst);

	int cs_set(cs* dst, const char* src);
	int cs_clear_length(cs* dst);
	int cs_append(cs* dst, const char* src);
	int cs_insert(cs* dst, const char* src, size_t pos);
	int cs_remove(cs* dst, size_t pos, size_t len);
	int cs_reset(cs* dst);

	int cs_find(cs* dst, const char* src);
	int cs_rfind(cs* dst, const char* src);

	int cs_equal_len(cs* a, cs* b);
	int cs_cmp_str(cs* a, cs* b);
	int cs_cmp_cstr(cs* a, const char* b);

	const char* cs_data(const cs* dst);
	size_t cs_length(const cs* dst);



#ifdef __cplusplus
}
#endif
#endif
