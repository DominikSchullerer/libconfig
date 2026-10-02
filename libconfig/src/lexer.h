#ifndef _LIBCONFIG_LEXER_H_
#define _LIBCONFIG_LEXER_H_

#include <stddef.h>

#include "token.h"

typedef struct lexer_t lexer_t;

typedef enum lexer_status_t {
	LEXER_OK,
	LEXER_INVALID_ARGUMENT,
	LEXER_OUT_OF_MEMORY
} lexer_status_t;

lexer_status_t lexer_create(
	const char *buffer, 
	lexer_t **out_lexer
);

void lexer_destroy(lexer_t *lexer);

token_t lexer_next_token(lexer_t *lexer);

#endif // !_LIBCONFIG_LEXER_H_
