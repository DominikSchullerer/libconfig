#ifndef _LIB_CONFIG_STRINGVIEW_H_
#define _LIB_CONFIG_STRINGVIEW_H_

#include <stddef.h>
#include <stdint.h>

typedef struct stringview_t stringview_t;

struct stringview_t {
	const char *data;
	size_t length;
};

#endif // !_LIB_CONFIG_STRINGVIEW_H_
