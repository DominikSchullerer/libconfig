#ifndef _LIBCONFIG_TOKEN_H_
#define _LIBCONFIG_TOKEN_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "stringview.h"

typedef struct token_t token_t;

typedef enum token_type_t {
	// Literals
	TOKEN_IDENTIFIER,
	TOKEN_STRING,
	TOKEN_INTEGER,
	TOKEN_FLOAT,
	TOKEN_TRUE,
	TOKEN_FALSE,

	// Delimiters
	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,
	TOKEN_LPAREN,
	TOKEN_RPAREN,

	// Punctuation
	TOKEN_EQUAL,
	TOKEN_COLON,
	TOKEN_SEMICOLON,
	TOKEN_COMMA,

	// Special
	TOKEN_EOF,
	TOKEN_ERROR
} token_type_t;

struct token_t {
	token_type_t type;

	stringview_t literal;

	size_t line;
	size_t column;
};

#endif // !_LIBCONFIG_TOKEN_H_
