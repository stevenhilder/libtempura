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

#include <stdbool.h> // for bool
#include <stddef.h> // for size_t
#include <stdio.h> // for stdout, stderr, fprintf()
#include <string.h> // for memset(), memcpy()

#include "tempura_lexer.h"

#include "tempura_ast.h" // for tempura_ast_node_allocate(), enum tempura_ast_node_type
#include "tempura_error.h" // for struct tempura_error, tempura_error_set()
#include "tempura_types.h" // for enum tempura_result

static inline bool tempura_is_identifier_start(char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || c == '-';
}

static enum tempura_result tempura_lexer_string_escape(struct tempura_lexer_context* lexer) {
	register uint8_t* in = lexer->token_content.bytes;
	register uint8_t* out = in;
	uint8_t* end = in + (size_t)lexer->token_content.length;
	while (in < end) {
		if (*in == '\\') {
			if (++in >= end) {
				*out++ = '\\';
				break;
			}
			switch (*in) {
				case 'n':
					*out++ = '\n';
					--lexer->token_content.length;
					break;
				case 'r':
					*out++ = '\r';
					--lexer->token_content.length;
					break;
				case 't':
					*out++ = '\t';
					--lexer->token_content.length;
					break;
				case '"':
				case '\\':
				case '$':
					*out++ = *in;
					--lexer->token_content.length;
					break;
				default:
					tempura_error_set(lexer->error, TEMPURA_ERROR_TYPE_INVALID_ESCAPE_SEQUENCE, "Invalid escape sequence \"\\%c\"", *in);
					return TEMPURA_RESULT_FAILURE;
			}
		} else {
			*out++ = *in;
		}
		in++;
	}
	*out = '\0'; // might not be necessary, since we won't be treating the array of bytes as a null-terminated C string
	return TEMPURA_RESULT_SUCCESS;
}

static inline void tempura_lexer_token_content_set(struct tempura_lexer_context* lexer, size_t offset, size_t delta) {
	lexer->token_content.length = (uint16_t)(lexer->cursor - lexer->token - delta);
	tempura_memory_allocate(lexer->memory, (void**)&lexer->token_content.bytes, lexer->token_content.length * sizeof (uint8_t));
	memcpy(lexer->token_content.bytes, lexer->token + offset, lexer->token_content.length);
}

#define TEMPURA_LEXER_TOKEN_CONTENT_SET(__lexer, __token_prefix, __token_suffix) \
	tempura_lexer_token_content_set((__lexer), (sizeof (__token_prefix) - 1), sizeof (__token_prefix) + sizeof (__token_suffix) - 2)

static int tempura_lexer_token(struct tempura_lexer_context* lexer) {
restart:
	lexer->token = lexer->cursor;

	/*!re2c
	re2c:condenumprefix = '';
	re2c:condprefix = '';
	re2c:define:YYCONDTYPE = 'tempura_lexer_condition';
	re2c:define:YYCTYPE = 'uint8_t';
	re2c:define:YYCURSOR = 'lexer->cursor';
	re2c:define:YYGETCONDITION = 'lexer->condition';
	re2c:define:YYGETCONDITION:naked = 1;
	re2c:define:YYLIMIT = 'lexer->limit';
	re2c:define:YYMARKER = 'lexer->marker';
	re2c:indent:top = 1;
	re2c:labelprefix = 're2c_';
	re2c:yyfill:enable = 0;

		tempura_lexer_rule_identifier = [0-9A-Za-z_\-]+;

		<TEMPURA_LEXER_CONDITION_NORMAL> [\t\n\r ]+ { goto restart; }
		<TEMPURA_LEXER_CONDITION_NORMAL> '//' { while (lexer->cursor < lexer->limit) { switch (*lexer->cursor++) { case '\r': if (*lexer->cursor == '\n') { ++lexer->cursor; } case '\n': break; default: continue; } break; } goto restart; }
		<TEMPURA_LEXER_CONDITION_NORMAL> '/*' { while (lexer->cursor < lexer->limit) { if (*lexer->cursor++ == '*' && *lexer->cursor == '/') { break; } } if (lexer->cursor < lexer->limit) { ++lexer->cursor; } else { tempura_error_set(lexer->error, TEMPURA_ERROR_TYPE_UNTERMINATED_COMMENT, "Unterminated comment"); return TEMPURA_LEXER_PSEUDOTOKEN_ERROR; } goto restart; }

		<TEMPURA_LEXER_CONDITION_NORMAL> '{' { return TEMPURA_LEXER_TOKEN_BRACE_LEFT; }
		<TEMPURA_LEXER_CONDITION_NORMAL> '}' { return TEMPURA_LEXER_TOKEN_BRACE_RIGHT; }
		<TEMPURA_LEXER_CONDITION_NORMAL> ':' { return TEMPURA_LEXER_TOKEN_COLON; }
		<TEMPURA_LEXER_CONDITION_NORMAL> ',' { return TEMPURA_LEXER_TOKEN_COMMA; }
		<TEMPURA_LEXER_CONDITION_NORMAL> '=' { return TEMPURA_LEXER_TOKEN_EQUALS; }
		<TEMPURA_LEXER_CONDITION_NORMAL> 'as' { return TEMPURA_LEXER_TOKEN_KEYWORD_AS; }
		<TEMPURA_LEXER_CONDITION_NORMAL> 'default' { return TEMPURA_LEXER_TOKEN_KEYWORD_DEFAULT; }
		<TEMPURA_LEXER_CONDITION_NORMAL> 'foreach' { return TEMPURA_LEXER_TOKEN_KEYWORD_FOREACH; }
		<TEMPURA_LEXER_CONDITION_NORMAL> 'function' { return TEMPURA_LEXER_TOKEN_KEYWORD_FUNCTION; }
		<TEMPURA_LEXER_CONDITION_NORMAL> 'import' { return TEMPURA_LEXER_TOKEN_KEYWORD_IMPORT; }
		<TEMPURA_LEXER_CONDITION_NORMAL> '(' { return TEMPURA_LEXER_TOKEN_PARENTHESIS_LEFT; }
		<TEMPURA_LEXER_CONDITION_NORMAL> ')' { return TEMPURA_LEXER_TOKEN_PARENTHESIS_RIGHT; }
		<TEMPURA_LEXER_CONDITION_NORMAL> ';' { return TEMPURA_LEXER_TOKEN_SEMICOLON; }

		<TEMPURA_LEXER_CONDITION_NORMAL> tempura_lexer_rule_identifier { TEMPURA_LEXER_TOKEN_CONTENT_SET(lexer, "", ""); return TEMPURA_LEXER_TOKEN_IDENTIFIER; }
		<TEMPURA_LEXER_CONDITION_NORMAL> '.' tempura_lexer_rule_identifier { TEMPURA_LEXER_TOKEN_CONTENT_SET(lexer, ".", ""); return TEMPURA_LEXER_TOKEN_HTML_SHORTHAND_ATTRIBUTE_CLASS; }
		<TEMPURA_LEXER_CONDITION_NORMAL> '#' tempura_lexer_rule_identifier { TEMPURA_LEXER_TOKEN_CONTENT_SET(lexer, "#", ""); return TEMPURA_LEXER_TOKEN_HTML_SHORTHAND_ATTRIBUTE_ID; }
		<TEMPURA_LEXER_CONDITION_NORMAL> '@' tempura_lexer_rule_identifier { TEMPURA_LEXER_TOKEN_CONTENT_SET(lexer, "@", ""); return TEMPURA_LEXER_TOKEN_HTML_ATTRIBUTE_NAME; }

		<TEMPURA_LEXER_CONDITION_NORMAL> '\'' { register uint8_t* in; register uint8_t* out; uint8_t* end; while (1) { if (lexer->cursor < lexer->limit) { if (*lexer->cursor == '\'') { ++lexer->cursor; break; } else if (*lexer->cursor++ == '\\' && lexer->cursor < lexer->limit) { ++lexer->cursor; } } else { tempura_error_set(lexer->error, TEMPURA_ERROR_TYPE_UNTERMINATED_STRING, "Unterminated string"); return TEMPURA_LEXER_PSEUDOTOKEN_ERROR; } } TEMPURA_LEXER_TOKEN_CONTENT_SET(lexer, "'", "'"); in = lexer->token_content.bytes; out = in; end = in + (size_t)lexer->token_content.length; while (in < end) { if (*in == '\\') { in++; switch (*in) { case '\\': case '\'': *out++ = *in; --lexer->token_content.length; break; default: *out++ = '\\'; *out++ = *in; break; } } else { *out++ = *in; } in++; } *out = '\0'; return TEMPURA_LEXER_TOKEN_STRING_LITERAL; }
		<TEMPURA_LEXER_CONDITION_NORMAL> '"' { while (lexer->cursor < lexer->limit) { switch (*lexer->cursor++) { case '"': TEMPURA_LEXER_TOKEN_CONTENT_SET(lexer, "\"", "\""); if (tempura_lexer_string_escape(lexer) == TEMPURA_RESULT_FAILURE) { return TEMPURA_LEXER_PSEUDOTOKEN_ERROR; } else { return TEMPURA_LEXER_TOKEN_STRING_LITERAL; } case '$': if (tempura_is_identifier_start(*lexer->cursor) || *lexer->cursor == '{') { break; } continue; case '\\': if (lexer->cursor < lexer->limit) { ++lexer->cursor; } default: continue; } --lexer->cursor; break; } lexer->fragment_len = lexer->cursor - lexer->token - 1; lexer->cursor = lexer->token + 1; tempura_lexer_condition_push(lexer, TEMPURA_LEXER_CONDITION_STRING_WITH_INTERPOLATION); return TEMPURA_LEXER_TOKEN_DOUBLE_QUOTE; }
		<TEMPURA_LEXER_CONDITION_NORMAL> '$' { tempura_lexer_condition_push(lexer, TEMPURA_LEXER_CONDITION_VARIABLE); goto restart; }

		<TEMPURA_LEXER_CONDITION_VARIABLE> tempura_lexer_rule_identifier { TEMPURA_LEXER_TOKEN_CONTENT_SET(lexer, "", ""); return TEMPURA_LEXER_TOKEN_VARIABLE_NAME; }
		<TEMPURA_LEXER_CONDITION_VARIABLE> '.' { tempura_lexer_condition_push(lexer, TEMPURA_LEXER_CONDITION_VARIABLE_PROPERTY); goto restart; }
		<TEMPURA_LEXER_CONDITION_VARIABLE> [^] { tempura_lexer_condition_pop(lexer); --lexer->cursor; goto restart; }

		<TEMPURA_LEXER_CONDITION_VARIABLE_PROPERTY> tempura_lexer_rule_identifier { TEMPURA_LEXER_TOKEN_CONTENT_SET(lexer, "", ""); return TEMPURA_LEXER_TOKEN_VARIABLE_PROPERTY; }
		<TEMPURA_LEXER_CONDITION_VARIABLE_PROPERTY> '.' { goto restart; }
		<TEMPURA_LEXER_CONDITION_VARIABLE_PROPERTY> [^] { tempura_lexer_condition_pop(lexer); --lexer->cursor; goto restart; }

		<TEMPURA_LEXER_CONDITION_STRING_WITH_INTERPOLATION> '$' { tempura_lexer_condition_push(lexer, TEMPURA_LEXER_CONDITION_VARIABLE); goto restart; }
		<TEMPURA_LEXER_CONDITION_STRING_WITH_INTERPOLATION> '${' { tempura_lexer_condition_push(lexer, TEMPURA_LEXER_CONDITION_STRING_WITH_INTERPOLATION_BRACES); tempura_lexer_condition_push(lexer, TEMPURA_LEXER_CONDITION_VARIABLE); goto restart; }
		<TEMPURA_LEXER_CONDITION_STRING_WITH_INTERPOLATION> '"' { tempura_lexer_condition_pop(lexer); return TEMPURA_LEXER_TOKEN_DOUBLE_QUOTE; }
		<TEMPURA_LEXER_CONDITION_STRING_WITH_INTERPOLATION> [^] { if (lexer->fragment_len > 0) { lexer->cursor += lexer->fragment_len - 1; lexer->fragment_len = 0; } else if (lexer->cursor > lexer->limit) { tempura_error_set(lexer->error, TEMPURA_ERROR_TYPE_UNTERMINATED_STRING, "Unterminated string"); return TEMPURA_LEXER_PSEUDOTOKEN_ERROR; } else if (*lexer->token == '\\' && lexer->cursor < lexer->limit) { ++lexer->cursor; } else { while (lexer->cursor < lexer->limit) { switch (*lexer->cursor++) { case '"': break; case '$': if (tempura_is_identifier_start(*lexer->cursor) || *lexer->cursor == '{') { break; } continue; case '\\': if (lexer->cursor < lexer->limit) { ++lexer->cursor; } default: continue; } --lexer->cursor; break; } } TEMPURA_LEXER_TOKEN_CONTENT_SET(lexer, "", ""); if (tempura_lexer_string_escape(lexer) == TEMPURA_RESULT_FAILURE) { return TEMPURA_LEXER_PSEUDOTOKEN_ERROR; } else { return TEMPURA_LEXER_TOKEN_STRING_LITERAL; } }

		<TEMPURA_LEXER_CONDITION_STRING_WITH_INTERPOLATION_BRACES> '}' { tempura_lexer_condition_pop(lexer); goto restart; }
		<TEMPURA_LEXER_CONDITION_STRING_WITH_INTERPOLATION_BRACES> [^] {  }

		// Any other character means either we're at the end of the stream or the template contains a syntax error
		<TEMPURA_LEXER_CONDITION_NORMAL> [^] {
			if (lexer->cursor > lexer->limit) {
				return TEMPURA_LEXER_PSEUDOTOKEN_END;
			} else {
				tempura_error_set(lexer->error, TEMPURA_ERROR_TYPE_UNEXPECTED_CHARACTER, "Unexpected character in input: '%c' (ASCII 0x%02x)", *lexer->token, (unsigned int)*lexer->token);
				return TEMPURA_LEXER_PSEUDOTOKEN_ERROR;
			}
		}
	*/
}

enum tempura_result tempura_lexer_initialize(struct tempura_lexer_context* lexer, struct tempura_memory_context* memory, char* buffer, size_t buffer_len) {
	memset(lexer, 0, sizeof (struct tempura_lexer_context));
	lexer->cursor = (uint8_t*)buffer;
	lexer->limit = lexer->cursor + buffer_len;
	tempura_lexer_condition_push(lexer, TEMPURA_LEXER_CONDITION_NORMAL);
	lexer->memory = memory;
	return TEMPURA_RESULT_SUCCESS;
}

void tempura_lexer_condition_push(struct tempura_lexer_context* lexer, enum tempura_lexer_condition condition) {
	lexer->condition = lexer->condition_stack.elements[lexer->condition_stack.size++] = condition;
}

void tempura_lexer_condition_pop(struct tempura_lexer_context* lexer) {
	lexer->condition = lexer->condition_stack.elements[--lexer->condition_stack.size - 1];
}

int tempura_lexer_tokenize(struct tempura_lexer_context* lexer, union tempura_semantic_value* value) {
	int token = tempura_lexer_token(lexer);
	enum tempura_ast_node_type ast_node_type = TEMPURA_AST_NODE_PSEUDOTYPE_NONE;
	switch (token) {
		case TEMPURA_LEXER_TOKEN_HTML_ATTRIBUTE_NAME: ast_node_type = TEMPURA_AST_NODE_TYPE_HTML_ATTRIBUTE_NAME; break;
		case TEMPURA_LEXER_TOKEN_HTML_SHORTHAND_ATTRIBUTE_CLASS: ast_node_type = TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_CLASS; break;
		case TEMPURA_LEXER_TOKEN_HTML_SHORTHAND_ATTRIBUTE_ID: ast_node_type = TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_ID; break;
		case TEMPURA_LEXER_TOKEN_IDENTIFIER: ast_node_type = TEMPURA_AST_NODE_TYPE_IDENTIFIER; break;
		case TEMPURA_LEXER_TOKEN_STRING_LITERAL: ast_node_type = TEMPURA_AST_NODE_TYPE_STRING_LITERAL; break;
		case TEMPURA_LEXER_TOKEN_VARIABLE_NAME: ast_node_type = TEMPURA_AST_NODE_TYPE_VARIABLE_NAME; break;
		case TEMPURA_LEXER_TOKEN_VARIABLE_PROPERTY: ast_node_type = TEMPURA_AST_NODE_TYPE_VARIABLE_PROPERTY; break;
	}
	if (ast_node_type != TEMPURA_AST_NODE_PSEUDOTYPE_NONE) {
		if (tempura_ast_node_allocate(lexer->memory, &value->ast, ast_node_type, 0, &lexer->token_content) == TEMPURA_RESULT_FAILURE) {
			tempura_error_set(lexer->error, TEMPURA_ERROR_TYPE_INTERNAL_ERROR, "Memory allocation error");
			return TEMPURA_LEXER_PSEUDOTOKEN_ERROR;
		}
	}
	return token;
}

void tempura_lexer_destroy(struct tempura_lexer_context* lexer) {
#	pragma message "Unused parameter 'lexer' in tempura_lexer_destroy()"
	(void)lexer;
	return;
}

static inline void tempura_lexer_token_print(uint16_t token) {
	switch (token) {
		#define X(__token) case __token: fprintf(stdout, #__token); break;
			TEMPURA_LEXER_PSEUDOTOKENS_XMACRO
			TEMPURA_LEXER_TOKENS_XMACRO
		#undef X
		default:
			fprintf(stderr, "UNKNOWN TOKEN!\n");
			break;
	}
}

enum tempura_result tempura_lexer_test(char* buffer, size_t buffer_len) {
	enum tempura_result retval = TEMPURA_RESULT_FAILURE;
	struct tempura_memory_context memory;
	if (tempura_memory_initialize(&memory, 4096) == TEMPURA_RESULT_FAILURE) {
		fprintf(stderr, "tempura_memory_initialize() failed\n");
	} else {
		struct tempura_error error;
		error.message.length = 4096;
		tempura_memory_allocate(&memory, (void**)&error.message.bytes, error.message.length);
		struct tempura_lexer_context lexer;
		if (tempura_lexer_initialize(&lexer, &memory, buffer, buffer_len) == TEMPURA_RESULT_FAILURE) {
			fprintf(stderr, "tempura_lexer_initialize() failed\n");
		} else {
			int token;
			lexer.error = &error;
			do {
				token = tempura_lexer_token(&lexer);
				tempura_lexer_token_print(token);
				switch (token) {
					case TEMPURA_LEXER_TOKEN_HTML_ATTRIBUTE_NAME:
					case TEMPURA_LEXER_TOKEN_HTML_SHORTHAND_ATTRIBUTE_CLASS:
					case TEMPURA_LEXER_TOKEN_HTML_SHORTHAND_ATTRIBUTE_ID:
					case TEMPURA_LEXER_TOKEN_IDENTIFIER:
					case TEMPURA_LEXER_TOKEN_STRING_LITERAL:
					case TEMPURA_LEXER_TOKEN_VARIABLE_NAME:
					case TEMPURA_LEXER_TOKEN_VARIABLE_PROPERTY:
						fprintf(stdout, " \"%.*s\"\n", (int)(uint16_t)lexer.token_content.length, (char*)lexer.token_content.bytes);
						break;
					default:
						fprintf(stdout, "\n");
				}
			} while (token != TEMPURA_LEXER_PSEUDOTOKEN_END && token != TEMPURA_LEXER_PSEUDOTOKEN_ERROR);
			if (token == TEMPURA_LEXER_PSEUDOTOKEN_ERROR) {
				fprintf(stderr, "Parse error: %.*s\n", (int)(uint16_t)lexer.error->message.length, (char*)lexer.error->message.bytes);
			} else {
				retval = TEMPURA_RESULT_SUCCESS;
			}
			tempura_lexer_destroy(&lexer);
		}
		tempura_memory_destroy(&memory);
	}
	return retval;
}
