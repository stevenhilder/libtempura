//
// libtempura - HTML templating engine
//
// Written in 2017 by Steven Hilder <steven.hilder@sevenpercent.solutions>
//
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with
// this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//

#include <stdio.h> // for fprintf(), fread(), stdin, stderr, stdout
#include <stdlib.h> // for malloc(), free(), EXIT_FAILURE, EXIT_SUCCESS

#include "tempura_parser.h"

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;
	#define BUFFER_SIZE ((size_t)4096)
	int retval = EXIT_FAILURE;
	char* buffer;
	if ((buffer = malloc(BUFFER_SIZE)) == NULL) {
		fprintf(stderr, "Error allocating buffer\n");
	} else {
		size_t len;
		if ((len = fread(buffer, sizeof (char), BUFFER_SIZE, stdin)) == BUFFER_SIZE && feof(stdin) == 0) {
			fprintf(stderr, "Input exceeds %zu-byte buffer\n", BUFFER_SIZE);
		} else if (ferror(stdin) != 0) {
			fprintf(stderr, "Error reading input\n");
		} else {
			retval = tempura_parser_test(buffer, len) == TEMPURA_RESULT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
		}
		free(buffer);
	}
	return retval;
	#undef BUFFER_SIZE
}
