#include <stdlib.h>

#include "lexer.h"

///////////////////////
// Type definitions
///////////////////////

struct lexer_t {
	const char *buffer;

	size_t index;
	size_t line;
	size_t column;

	size_t token_start;
	size_t token_line;
	size_t token_column;
};

///////////////////////
// Function declarations
///////////////////////

static char peek(lexer_t *lexer);
static token_t make_token(lexer_t *lexer, token_type_t type);

///////////////////////
// API functions
///////////////////////

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
    lexer->token_column = lexer->column;
    lexer->token_line = lexer->line;
    lexer->token_start = lexer->index;

    if (peek(lexer) == '\0') {
        return make_token(lexer, TOKEN_EOF);
    }

	return make_token(lexer, TOKEN_ERROR);
}

///////////////////////
// Static functions
///////////////////////

static char peek(lexer_t *lexer)
{
    return lexer->buffer[lexer->index];
}

static token_t make_token(lexer_t *lexer, token_type_t type)
{
    size_t length = lexer->index - lexer->token_start;
    stringview_t literal = { 
        .data = &lexer->buffer[lexer->token_start], 
        .length = length 
    };

    return (token_t) {
        .type = type,
        .literal = literal,
        .line = lexer->token_line,
        .column = lexer->token_column
    };
}

