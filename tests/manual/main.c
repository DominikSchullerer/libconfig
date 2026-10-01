#include <stdlib.h>
#include <string.h>

#include "lexer.h"

int main(void)
{
	char buffer[] = " \n";
	lexer_t *lexer;
	lexer_create(buffer, &lexer);

	while (1) {
		token_t token = lexer_next_token(lexer);

		if (token.type == TOKEN_EOF) {
			break;
		}
		if (token.type == TOKEN_ERROR) {
			break;
		}
	}

	return EXIT_SUCCESS;
}