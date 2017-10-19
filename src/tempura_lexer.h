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

#ifndef TEMPURA_LEXER_H
#define TEMPURA_LEXER_H

#include <stddef.h> // for size_t
#include <stdint.h> // for uint8_t, uint_fast8_t

#include "tempura_ast.h" // for union tempura_semantic_value
#include "tempura_error.h" // for struct tempura_error
#include "tempura_memory.h" // for struct tempura_memory_context
#include "tempura_types.h" // for enum tempura_result, struct tempura_string

enum tempura_lexer_condition {
	TEMPURA_LEXER_CONDITION_NORMAL,
	TEMPURA_LEXER_CONDITION_VARIABLE,
	TEMPURA_LEXER_CONDITION_VARIABLE_PROPERTY,
	TEMPURA_LEXER_CONDITION_STRING_WITH_INTERPOLATION,
	TEMPURA_LEXER_CONDITION_STRING_WITH_INTERPOLATION_BRACES,
};

enum tempura_lexer_token {
	TEMPURA_LEXER_PSEUDOTOKEN_END = 0,
	TEMPURA_LEXER_PSEUDOTOKEN_ERROR = 258,
	#define X(__token) __token,
		#include "tempura_lexer_tokens.h"
		TEMPURA_LEXER_TOKENS_XMACRO
	#undef X
};

struct tempura_lexer_context {
	uint8_t* cursor;
	uint8_t* token;
	uint8_t* limit;
	uint8_t* marker;
	enum tempura_lexer_condition condition;
	struct {
		uint_fast8_t size;
		enum tempura_lexer_condition elements[128];
	} condition_stack;
	size_t fragment_len;
	struct tempura_memory_context* memory;
	struct tempura_string token_content;
	struct tempura_error* error;
};

enum tempura_result tempura_lexer_initialize(struct tempura_lexer_context* lexer, struct tempura_memory_context* memory, char* buffer, size_t buffer_len);
void tempura_lexer_condition_push(struct tempura_lexer_context* lexer, enum tempura_lexer_condition condition);
void tempura_lexer_condition_pop(struct tempura_lexer_context* lexer);
int tempura_lexer_tokenize(struct tempura_lexer_context* lexer, union tempura_semantic_value* value);
void tempura_lexer_destroy(struct tempura_lexer_context* lexer);

enum tempura_result tempura_lexer_test(char* buffer, size_t buffer_len);

#endif
