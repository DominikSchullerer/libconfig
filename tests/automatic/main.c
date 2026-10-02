#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "test_lexer.h"


int main(void)
{
    if (!test_lexer()) {
        printf("Lexer tests failed\n");
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}
