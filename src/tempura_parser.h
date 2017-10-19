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

#ifndef TEMPURA_PARSER_H
#define TEMPURA_PARSER_H

#include "tempura_ast.h" // for struct tempura_ast_node
#include "tempura_error.h" // for struct tempura_error
#include "tempura_lexer.h" // for struct tempura_lexer_context
#include "tempura_memory.h" // for struct tempura_memory_context
#include "tempura_types.h" // for enum tempura_result

struct tempura_parser_context {
	struct tempura_memory_context* memory;
	struct tempura_lexer_context* lexer;
	struct tempura_ast_node* ast;
	struct tempura_error* error;
};

int tempura_parse(struct tempura_parser_context* parser);
void tempura_parser_error_set(struct tempura_parser_context* parser, const char* message);

enum tempura_result tempura_parser_test(char* buffer, size_t buffer_len);

#endif
