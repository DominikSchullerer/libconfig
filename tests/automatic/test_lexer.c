#include <stdio.h>
#include <string.h>

#include "test_lexer.h"

#include "lexer.h"

////////////////////////////////
// Function declarations
////////////////////////////////

static bool test_lexer_create(void);
static bool test_lexer_create_invalid_arguments(void);
static bool test_lexer_empty_buffer(void);
static bool test_lexer_token_error(void);
static bool test_lexer_token_delimiters(void);
static bool test_lexer_token_punctuation(void);
static bool test_lexer_skip_whitespace(void);
static bool test_lexer_skip_comments(void);
static bool test_lexer_skip_whitespace_and_comments(void);
static bool test_lexer_strings(void);

static bool match_token(
    token_t a,
    token_type_t type,
    size_t line,
    size_t column,
    const char *literal
);

////////////////////////////////
// Test suite
////////////////////////////////

bool test_lexer(void)
{
    printf("Running lexer tests...\n");

    if (!test_lexer_create()) {
        printf("test_lexer_create failed\n");
        return false;
    }

    if (!test_lexer_create_invalid_arguments()) {
        printf("test_lexer_create_invalid_arguments failed\n");
        return false;
    }

    if (!test_lexer_empty_buffer()) {
        printf("test_lexer_empty_buffer failed\n");
        return false;
    }

    if (!test_lexer_token_error()) {
        printf("test_lexer_token_error failed\n");
        return false;
    }

    if (!test_lexer_token_delimiters()) {
        printf("test_lexer_token_delimiters failed\n");
        return false;
    }

    if (!test_lexer_token_punctuation()) {
        printf("test_lexer_token_punctuation failed\n");
        return false;
    }

    if (!test_lexer_skip_whitespace()) {
        printf("test_lexer_skip_whitespace failed\n");
        return false;
    }

    if (!test_lexer_skip_comments()) {
        printf("test_lexer_skip_comments failed\n");
        return false;
    }

    if (!test_lexer_skip_whitespace_and_comments()) {
        printf("test_lexer_skip_whitespace_and_comments failed\n");
        return false;
    }

    if (!test_lexer_strings()) {
        printf("test_lexer_strings failed\n");
        return false;
    }

    printf("All lexer tests passed!\n");

    return true;
}

////////////////////////////////
// Test cases
////////////////////////////////

static bool test_lexer_create(void)
{
    lexer_t *lexer = NULL;
    lexer_status_t status = lexer_create("test", &lexer);
    if (status != LEXER_OK || lexer == NULL) {
        printf("Expected LEXER_OK and non-NULL lexer, got %d and %p\n", status, (void *)lexer);
        return false;
    }

    lexer_destroy(lexer);
    return true;
}

static bool test_lexer_create_invalid_arguments(void)
{
    lexer_t *lexer = NULL;
    lexer_status_t status = lexer_create(NULL, &lexer);
    if (status != LEXER_INVALID_ARGUMENT) {
        printf("Expected LEXER_INVALID_ARGUMENT, got %d\n", status);
        return false;
    }

    status = lexer_create("test", NULL);
    if (status != LEXER_INVALID_ARGUMENT) {
        printf("Expected LEXER_INVALID_ARGUMENT, got %d\n", status);
        return false;
    }

    return true;
}

static bool test_lexer_empty_buffer(void)
{
    lexer_t *lexer = NULL;
    lexer_status_t status = lexer_create("", &lexer);
    if (status != LEXER_OK || lexer == NULL) {
        printf("Expected LEXER_OK and non-NULL lexer, got %d and %p\n", status, (void *)lexer);
        return false;
    }

    token_t token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_EOF, 1, 1, "") == false) {
        lexer_destroy(lexer);
        return false;
    }

    lexer_destroy(lexer);
    return true;
}

static bool test_lexer_token_error(void)
{
    lexer_t *lexer = NULL;
    lexer_status_t status = lexer_create("~", &lexer);
    if (status != LEXER_OK || lexer == NULL) {
        printf("Expected LEXER_OK and non-NULL lexer, got %d and %p\n", status, (void *)lexer);
        return false;
    }

    token_t token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_ERROR, 1, 1, "") == false) {
        lexer_destroy(lexer);
        return false;
    }

    lexer_destroy(lexer);
    return true;
}

static bool test_lexer_token_delimiters(void)
{
    lexer_t *lexer = NULL;
    lexer_status_t status = lexer_create("{}[]()", &lexer);
    if (status != LEXER_OK || lexer == NULL) {
        printf("Expected LEXER_OK and non-NULL lexer, got %d and %p\n", status, (void *)lexer);
        return false;
    }

    token_t token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_LBRACE, 1, 1, "{") == false) {
        lexer_destroy(lexer);
        return false;
    }
    token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_RBRACE, 1, 2, "}") == false) {
        lexer_destroy(lexer);
        return false;
    }
    token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_LBRACKET, 1, 3, "[") == false) {
        lexer_destroy(lexer);
        return false;
    }
    token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_RBRACKET, 1, 4, "]") == false) {
        lexer_destroy(lexer);
        return false;
    }
    token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_LPAREN, 1, 5, "(") == false) {
        lexer_destroy(lexer);
        return false;
    }
    token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_RPAREN, 1, 6, ")") == false) {
        lexer_destroy(lexer);
        return false;
    }
    token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_EOF, 1, 7, "") == false) {
        lexer_destroy(lexer);
        return false;
    }
    lexer_destroy(lexer);
    return true;
}

static bool test_lexer_token_punctuation(void)
{
    lexer_t *lexer = NULL;
    lexer_status_t status = lexer_create("=:;,", &lexer);
    if (status != LEXER_OK || lexer == NULL) {
        printf("Expected LEXER_OK and non-NULL lexer, got %d and %p\n", status, (void *)lexer);
        return false;
    }
    token_t token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_EQUAL, 1, 1, "=") == false) {
        lexer_destroy(lexer);
        return false;
    }
    token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_COLON, 1, 2, ":") == false) {
        lexer_destroy(lexer);
        return false;
    }
    token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_SEMICOLON, 1, 3, ";") == false) {
        lexer_destroy(lexer);
        return false;
    }
    token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_COMMA, 1, 4, ",") == false) {
        lexer_destroy(lexer);
        return false;
    }
    token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_EOF, 1, 5, "") == false) {
        lexer_destroy(lexer);
        return false;
    }
    lexer_destroy(lexer);
    return true;
}

static bool test_lexer_skip_whitespace(void)
{
    lexer_t *lexer = NULL;
    lexer_status_t status = lexer_create("   \n\t  ", &lexer);
    if (status != LEXER_OK || lexer == NULL) {
        printf("Expected LEXER_OK and non-NULL lexer, got %d and %p\n", status, (void *)lexer);
        return false;
    }
    token_t token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_EOF, 2, 4, "") == false) {
        lexer_destroy(lexer);
        return false;
    }
    lexer_destroy(lexer);
    return true;
}

static bool test_lexer_skip_comments(void)
{
    lexer_t *lexer = NULL;
    lexer_status_t status = lexer_create("# This is a comment", &lexer);
    if (status != LEXER_OK || lexer == NULL) {
        printf("Expected LEXER_OK and non-NULL lexer, got %d and %p\n", status, (void *)lexer);
        return false;
    }
    token_t token = lexer_next_token(lexer);

    if (match_token(token, TOKEN_EOF, 1, 20, "") == false) {
        lexer_destroy(lexer);
        return false;
    }
    lexer_destroy(lexer);
    return true;
}

static bool test_lexer_skip_whitespace_and_comments(void)
{
    lexer_t *lexer = NULL;
    lexer_status_t status = lexer_create("   # This is a comment\n #This is another comment  ", &lexer);
    if (status != LEXER_OK || lexer == NULL) {
        printf("Expected LEXER_OK and non-NULL lexer, got %d and %p\n", status, (void *)lexer);
        return false;
    }   
    token_t token = lexer_next_token(lexer);

    if (match_token(token, TOKEN_EOF, 2, 28, "") == false) {
        lexer_destroy(lexer);
        return false;
    }
    lexer_destroy(lexer);
    return true;
}

static bool test_lexer_strings(void)
{
    lexer_t *lexer = NULL;
    lexer_status_t status = lexer_create("\n\"This is a string\"", &lexer);
    if (status != LEXER_OK || lexer == NULL) {
        printf("Expected LEXER_OK and non-NULL lexer, got %d and %p\n", status, (void *) lexer);
        return false;
    }

    token_t token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_STRING, 2, 1, "\"This is a string\"") == false) {
        lexer_destroy(lexer);
        return false;
    }

    token = lexer_next_token(lexer);
    if (match_token(token, TOKEN_EOF, 2, 19, "") == false) {
        lexer_destroy(lexer);
        return false;
    }

    lexer_destroy(lexer);

    return true;
}

////////////////////////////////
// Helper functions
////////////////////////////////

static bool match_token(
    token_t a, 
    token_type_t type, 
    size_t line, 
    size_t column, 
    const char *literal)
{
    if (a.type != type) {
        fprintf(stderr,
                "Token types differ: %d vs %d\n",
                a.type, type);
        return false;
    }
    if (a.line != line) {
        fprintf(stderr,
                "Token lines differ: %zu vs %zu\n",
                a.line, line);
        return false;
    }
    if (a.column != column) {
        fprintf(stderr,
                "Token columns differ: %zu vs %zu\n",
                a.column, column);
        return false;
    }
    if (a.literal.length != strlen(literal)) {
        fprintf(stderr,
                "Token literal lengths differ: %zu vs %zu\n",
                a.literal.length, strlen(literal));
        return false;
    }
    if (strncmp(a.literal.data, literal, a.literal.length) != 0) {
        fprintf(stderr,
                "Token literal data differ: '%.*s' vs '%s'\n",
                (int)a.literal.length, a.literal.data, literal);
        return false;
    }
    return true;
}