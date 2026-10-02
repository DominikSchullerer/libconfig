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

static char lexer_peek(lexer_t *lexer);
static char lexer_advance(lexer_t *lexer);
static char lexer_is_whitespace(char c);

static void lexer_skip_trivia(lexer_t *lexer);

static bool lexer_scan_delimiter(lexer_t *lexer, token_t *token);
static bool lexer_scan_punctuation(lexer_t *lexer, token_t *token);

static token_t lexer_make_token(lexer_t *lexer, token_type_t type);

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
    lexer_skip_trivia(lexer);

    lexer->token_column = lexer->column;
    lexer->token_line = lexer->line;
    lexer->token_start = lexer->index;

    if (lexer_peek(lexer) == '\0') {
        return lexer_make_token(lexer, TOKEN_EOF);
    }

    token_t token;

    if (lexer_scan_delimiter(lexer, &token)) {
        return token;
    }

    if (lexer_scan_punctuation(lexer, &token)) {
        return token;
    }

	return lexer_make_token(lexer, TOKEN_ERROR);
}

///////////////////////
// Static functions
///////////////////////

static char lexer_peek(lexer_t *lexer)
{
    return lexer->buffer[lexer->index];
}

static char lexer_advance(lexer_t *lexer)
{
    char c = lexer_peek(lexer);
    if (c == '\0') {
        return c;
    }

    lexer->index++;

    if (c == '\n') {
        lexer->line += 1;
        lexer->column = 1;
    }
    else {
        lexer->column += 1;
    }
    
    return c;
}

static char lexer_is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static void lexer_skip_trivia(lexer_t *lexer)
{
    while (lexer_peek(lexer) != '\0') {
        if (lexer_is_whitespace(lexer_peek(lexer))) {
            lexer_advance(lexer);
            continue;
        }

        if (lexer_peek(lexer) == '#') {
            while (lexer_peek(lexer) != '\0' && lexer_peek(lexer) != '\n') {
                lexer_advance(lexer);
            }
            continue;
        }

        return;
    }
}

static bool lexer_scan_delimiter(lexer_t *lexer, token_t *token)
{
    char c = lexer_peek(lexer);
    token_type_t type;

    switch (c) {
        case '{':
            type = TOKEN_LBRACE;
            break;
        case '}':
            type = TOKEN_RBRACE;
            break;
        case '[':
            type = TOKEN_LBRACKET;
            break;
        case ']':
            type = TOKEN_RBRACKET;
            break;
        case '(':
            type = TOKEN_LPAREN;
            break;
        case ')':
            type = TOKEN_RPAREN;
            break;
        default:
            return false;
    }

    lexer_advance(lexer);
    *token = lexer_make_token(lexer, type);
    return true;;
}

static bool lexer_scan_punctuation(lexer_t *lexer, token_t *token)
{
    char c = lexer_peek(lexer);
    token_type_t type;
    switch (c) {
        case '=':
            type = TOKEN_EQUAL;
            break;
        case ':':
            type = TOKEN_COLON;
            break;
        case ';':
            type = TOKEN_SEMICOLON;
            break;
        case ',':
            type = TOKEN_COMMA;
            break;
        default:
            return false;
    }

    lexer_advance(lexer);
    *token = lexer_make_token(lexer, type);
    return true;
}

static token_t lexer_make_token(lexer_t *lexer, token_type_t type)
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

