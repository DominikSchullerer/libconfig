#include <stdlib.h>

#include "lexer.h"


struct lexer_t {
	const char *buffer;

	size_t index;
	size_t line;
	size_t column;

	size_t token_start;
	size_t token_line;
	size_t token_column;
};


lexer_status_t lexer_create(
	const char *buffer,
	lexer_t **out_lexer
)
{
	if (buffer == NULL || out_lexer == NULL) {
		return LEXER_INVALID_ARGUMENT;
	}

	lexer_t *lexer = malloc(sizeof(*lexer));
	if (!lexer) {
		return LEXER_OUT_OF_MEMORY;
	}

	lexer->buffer = buffer;
	lexer->index = 0;
	lexer->line = 1;
	lexer->column = 1;

	*out_lexer = lexer;

	return LEXER_OK;
}

void lexer_destroy(lexer_t *lexer)
{
	free(lexer);
}

// TODO
token_t lexer_next_token(lexer_t *lexer)
{
    (void)lexer;
	return (token_t) { 0 };
}

