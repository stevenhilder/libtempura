%code requires {

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

#include <inttypes.h> // for PRIu16
#include <stdio.h> // for stdout, stderr, FILE, fprintf()
#include <string.h> // for memset()

#include "tempura_parser.h"

#include "tempura_ast.h" // for tempura_ast_*()
#include "tempura_error.h"
#include "tempura_lexer.h" // for tempura_lex()

#define bison_error tempura_parser_error_set
#define bison_lex(__semantic_value) tempura_lexer_tokenize(parser->lexer, __semantic_value)
#define bison_parse tempura_parse
#define bison_tokentype tempura_parser_token_type

#define BISON_TOKENTYPE

}

%require "3.0.4"

%define api.prefix {bison_}
%define api.pure full
%define api.push-pull pull
%define api.token.prefix {}
%define api.value.type {union tempura_semantic_value}
%define lr.default-reduction most
%define lr.keep-unreachable-state false
%define lr.type lalr
%define parse.lac full
%define parse.trace false
%expect 0

%no-lines

%parse-param {struct tempura_parser_context* parser}

%destructor {
	tempura_ast_node_free(parser->memory, &$$);
} <ast>

%token TEMPURA_LEXER_PSEUDOTOKEN_END 0 "(end of file)"
%token TEMPURA_LEXER_PSEUDOTOKEN_ERROR
%token TEMPURA_LEXER_TOKEN_BRACE_LEFT "'{'"
%token TEMPURA_LEXER_TOKEN_BRACE_RIGHT "'}'"
%token TEMPURA_LEXER_TOKEN_COLON "':'"
%token TEMPURA_LEXER_TOKEN_COMMA "','"
%token TEMPURA_LEXER_TOKEN_DOUBLE_QUOTE "'\"'"
%token TEMPURA_LEXER_TOKEN_EQUALS "'='"
%token <ast> TEMPURA_LEXER_TOKEN_HTML_ATTRIBUTE_NAME "(HTML attribute)"
%token <ast> TEMPURA_LEXER_TOKEN_HTML_SHORTHAND_ATTRIBUTE_CLASS "(HTML class attribute)"
%token <ast> TEMPURA_LEXER_TOKEN_HTML_SHORTHAND_ATTRIBUTE_ID "(HTML id attribute)"
%token <ast> TEMPURA_LEXER_TOKEN_IDENTIFIER "(identifier)"
%token TEMPURA_LEXER_TOKEN_KEYWORD_AS "'as'"
%token TEMPURA_LEXER_TOKEN_KEYWORD_DEFAULT "'default'"
%token TEMPURA_LEXER_TOKEN_KEYWORD_FOREACH "'foreach'"
%token TEMPURA_LEXER_TOKEN_KEYWORD_FUNCTION "'function'"
%token TEMPURA_LEXER_TOKEN_KEYWORD_IMPORT "'import'"
%token TEMPURA_LEXER_TOKEN_PARENTHESIS_LEFT "'('"
%token TEMPURA_LEXER_TOKEN_PARENTHESIS_RIGHT "')'"
%token TEMPURA_LEXER_TOKEN_SEMICOLON "';'"
%token <ast> TEMPURA_LEXER_TOKEN_STRING_LITERAL "(string literal)"
%token <ast> TEMPURA_LEXER_TOKEN_VARIABLE_NAME "(variable name)"
%token <ast> TEMPURA_LEXER_TOKEN_VARIABLE_PROPERTY "(property)"

%type <ast> tempura_parser_rule_argument
%type <ast> tempura_parser_rule_argument_list
%type <ast> tempura_parser_rule_argument_list_non_empty
%type <ast> tempura_parser_rule_expression
%type <ast> tempura_parser_rule_foreach
%type <ast> tempura_parser_rule_foreach_body
%type <ast> tempura_parser_rule_function_body
%type <ast> tempura_parser_rule_function_call
%type <ast> tempura_parser_rule_function_declaration
%type <ast> tempura_parser_rule_function_name
%type <ast> tempura_parser_rule_html_attribute
%type <ast> tempura_parser_rule_html_attribute_name
%type <ast> tempura_parser_rule_html_children
%type <ast> tempura_parser_rule_html_element
%type <ast> tempura_parser_rule_html_shorthand_attribute
%type <ast> tempura_parser_rule_html_shorthand_attribute_class
%type <ast> tempura_parser_rule_html_shorthand_attribute_id
%type <ast> tempura_parser_rule_html_shorthand_attribute_list
%type <ast> tempura_parser_rule_html_tag_name
%type <ast> tempura_parser_rule_identifier
%type <ast> tempura_parser_rule_import
%type <ast> tempura_parser_rule_parameter
%type <ast> tempura_parser_rule_parameter_list
%type <ast> tempura_parser_rule_parameter_list_non_empty
%type <ast> tempura_parser_rule_statement
%type <ast> tempura_parser_rule_statement_list
%type <ast> tempura_parser_rule_string
%type <ast> tempura_parser_rule_string_interpolation_list
%type <ast> tempura_parser_rule_string_literal
%type <ast> tempura_parser_rule_string_with_interpolation
%type <ast> tempura_parser_rule_top_statement
%type <ast> tempura_parser_rule_top_statement_list
%type <ast> tempura_parser_rule_variable
%type <ast> tempura_parser_rule_variable_name
%type <ast> tempura_parser_rule_variable_property
%type <ast> tempura_parser_rule_variable_property_list

%%

start:
	/* $1 */ tempura_parser_rule_top_statement_list
	{ parser->ast = $1; };

tempura_parser_rule_variable_name:
	/* $1 */ TEMPURA_LEXER_TOKEN_VARIABLE_NAME
	{ $$ = $1; };

tempura_parser_rule_variable_property:
	/* $1 */ TEMPURA_LEXER_TOKEN_VARIABLE_PROPERTY
	{ $$ = $1; };

tempura_parser_rule_variable_property_list:
	/* $1 */ tempura_parser_rule_variable_property_list
	/* $2 */ tempura_parser_rule_variable_property
	{ tempura_ast_node_append(parser->memory, $1, $2); $$ = $1; } |
	/* empty */
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_VARIABLE_PROPERTY_LIST, 0); };

tempura_parser_rule_variable:
	/* $1 */ tempura_parser_rule_variable_name
	/* $2 */ tempura_parser_rule_variable_property_list
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_VARIABLE, 2, $1, $2); };

tempura_parser_rule_string_literal:
	/* $1 */ TEMPURA_LEXER_TOKEN_STRING_LITERAL
	{ $$ = $1; };

tempura_parser_rule_string_interpolation_list:
	/* $1 */ tempura_parser_rule_string_interpolation_list
	/* $2 */ tempura_parser_rule_variable
	{ tempura_ast_node_append(parser->memory, $1, $2); $$ = $1; } |
	/* $1 */ tempura_parser_rule_string_interpolation_list
	/* $2 */ tempura_parser_rule_string_literal
	{ tempura_ast_node_append(parser->memory, $1, $2); $$ = $1; } |
	/* $1 */ tempura_parser_rule_variable
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_STRING_INTERPOLATION_LIST, 1, $1); } |
	/* $1 */ tempura_parser_rule_string_literal
	/* $2 */ tempura_parser_rule_variable
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_STRING_INTERPOLATION_LIST, 2, $1, $2); };

tempura_parser_rule_string_with_interpolation:
	/* $1 */ TEMPURA_LEXER_TOKEN_DOUBLE_QUOTE
	/* $2 */ tempura_parser_rule_string_interpolation_list
	/* $3 */ TEMPURA_LEXER_TOKEN_DOUBLE_QUOTE
	{ $$ = $2; };

tempura_parser_rule_string:
	/* $1 */ tempura_parser_rule_string_literal
	{ $$ = $1; } |
	/* $1 */ tempura_parser_rule_string_with_interpolation
	{ $$ = $1; };

tempura_parser_rule_identifier:
	/* $1 */ TEMPURA_LEXER_TOKEN_IDENTIFIER
	{ $$ = $1; };

tempura_parser_rule_function_name:
	/* $1 */ tempura_parser_rule_identifier
	{ $$ = $1; };

tempura_parser_rule_argument:
	/* $1 */ TEMPURA_LEXER_TOKEN_KEYWORD_DEFAULT
	{ $$ = NULL; } |
	/* $1 */ tempura_parser_rule_expression
	{ $$ = $1; };

tempura_parser_rule_argument_list_non_empty:
	/* $1 */ tempura_parser_rule_argument_list_non_empty
	/* $2 */ TEMPURA_LEXER_TOKEN_COMMA
	/* $3 */ tempura_parser_rule_argument
	{ tempura_ast_node_append(parser->memory, $1, $3); $$ = $1; } |
	/* $1 */ tempura_parser_rule_argument
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_ARGUMENT_LIST, 1, $1); };

tempura_parser_rule_argument_list:
	/* $1 */ tempura_parser_rule_argument_list_non_empty
	{ $$ = $1; } |
	/* empty */
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_ARGUMENT_LIST, 0); };

tempura_parser_rule_function_call:
	/* $1 */ tempura_parser_rule_function_name
	/* $2 */ TEMPURA_LEXER_TOKEN_PARENTHESIS_LEFT
	/* $3 */ tempura_parser_rule_argument_list
	/* $4 */ TEMPURA_LEXER_TOKEN_PARENTHESIS_RIGHT
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_FUNCTION_CALL, 2, $1, $3); };

tempura_parser_rule_expression:
	/* $1 */ tempura_parser_rule_string
	{ $$ = $1; } |
	/* $1 */ tempura_parser_rule_variable
	{ $$ = $1; } |
	/* $1 */ tempura_parser_rule_function_call
	{ $$ = $1; };

tempura_parser_rule_html_attribute_name:
	/* $1 */ TEMPURA_LEXER_TOKEN_HTML_ATTRIBUTE_NAME
	{ $$ = $1; };

tempura_parser_rule_html_attribute:
	/* $1 */ tempura_parser_rule_html_attribute_name
	/* $2 */ TEMPURA_LEXER_TOKEN_COLON
	/* $3 */ tempura_parser_rule_expression
	/* $4 */ TEMPURA_LEXER_TOKEN_SEMICOLON
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_HTML_ATTRIBUTE, 2, $1, $3); };

tempura_parser_rule_html_shorthand_attribute_class:
	/* $1 */ TEMPURA_LEXER_TOKEN_HTML_SHORTHAND_ATTRIBUTE_CLASS
	{ $$ = $1; };

tempura_parser_rule_html_shorthand_attribute_id:
	/* $1 */ TEMPURA_LEXER_TOKEN_HTML_SHORTHAND_ATTRIBUTE_ID
	{ $$ = $1; };

tempura_parser_rule_html_shorthand_attribute:
	/* $1 */ tempura_parser_rule_html_shorthand_attribute_class
	{ $$ = $1; } |
	/* $1 */ tempura_parser_rule_html_shorthand_attribute_id
	{ $$ = $1; };

tempura_parser_rule_html_shorthand_attribute_list:
	/* $1 */ tempura_parser_rule_html_shorthand_attribute_list
	/* $2 */ tempura_parser_rule_html_shorthand_attribute
	{ tempura_ast_node_append(parser->memory, $1, $2); $$ = $1; } |
	/* empty */
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_LIST, 0); };

tempura_parser_rule_html_tag_name:
	/* $1 */ tempura_parser_rule_identifier
	{ $$ = $1; };

tempura_parser_rule_html_children:
	/* $1 */ TEMPURA_LEXER_TOKEN_BRACE_LEFT
	/* $2 */ tempura_parser_rule_statement_list
	/* $3 */ TEMPURA_LEXER_TOKEN_BRACE_RIGHT
	{ $$ = $2; } |
	/* $1 */ tempura_parser_rule_statement
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_STATEMENT_LIST, 1, $1); } |
	/* $1 */ TEMPURA_LEXER_TOKEN_SEMICOLON
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_STATEMENT_LIST, 0); };

tempura_parser_rule_html_element:
	/* $1 */ tempura_parser_rule_html_tag_name
	/* $2 */ tempura_parser_rule_html_shorthand_attribute_list
	/* $3 */ tempura_parser_rule_html_children
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_HTML_ELEMENT, 3, $1, $2, $3); };

tempura_parser_rule_statement:
	/* $1 */ tempura_parser_rule_expression
	/* $2 */ TEMPURA_LEXER_TOKEN_SEMICOLON
	{ $$ = $1; } |
	/* $1 */ tempura_parser_rule_foreach
	{ $$ = $1; } |
	/* $1 */ tempura_parser_rule_html_element
	{ $$ = $1; } |
	/* $1 */ tempura_parser_rule_html_attribute
	{ $$ = $1; };

tempura_parser_rule_statement_list:
	/* $1 */ tempura_parser_rule_statement_list
	/* $2 */ tempura_parser_rule_statement
	{ tempura_ast_node_append(parser->memory, $1, $2); $$ = $1; } |
	/* empty */
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_STATEMENT_LIST, 0); };

tempura_parser_rule_foreach_body:
	/* $1 */ tempura_parser_rule_statement_list
	{ $$ = $1; };

tempura_parser_rule_foreach:
	/* $1 */ TEMPURA_LEXER_TOKEN_KEYWORD_FOREACH
	/* $2 */ TEMPURA_LEXER_TOKEN_PARENTHESIS_LEFT
	/* $3 */ tempura_parser_rule_variable
	/* $4 */ TEMPURA_LEXER_TOKEN_KEYWORD_AS
	/* $5 */ tempura_parser_rule_variable_name
	/* $6 */ TEMPURA_LEXER_TOKEN_PARENTHESIS_RIGHT
	/* $7 */ TEMPURA_LEXER_TOKEN_BRACE_LEFT
	/* $8 */ tempura_parser_rule_foreach_body
	/* $9 */ TEMPURA_LEXER_TOKEN_BRACE_RIGHT
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_FOREACH, 3, $3, $5, $8); };

tempura_parser_rule_parameter:
	/* $1 */ tempura_parser_rule_variable_name
	/* $2 */ TEMPURA_LEXER_TOKEN_EQUALS
	/* $3 */ tempura_parser_rule_string_literal
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_PARAMETER, 2, $1, $3); } |
	/* $1 */ tempura_parser_rule_variable_name
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_PARAMETER, 2, $1, NULL); };

tempura_parser_rule_parameter_list_non_empty:
	/* $1 */ tempura_parser_rule_parameter_list_non_empty
	/* $2 */ TEMPURA_LEXER_TOKEN_COMMA
	/* $3 */ tempura_parser_rule_parameter
	{ tempura_ast_node_append(parser->memory, $1, $3); $$ = $1; } |
	/* $1 */ tempura_parser_rule_parameter
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_PARAMETER_LIST, 1, $1); };

tempura_parser_rule_parameter_list:
	/* $1 */ tempura_parser_rule_parameter_list_non_empty
	{ $$ = $1; } |
	/* empty */
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_PARAMETER_LIST, 0); };

tempura_parser_rule_function_body:
	/* $1 */ tempura_parser_rule_statement_list
	{ $$ = $1; };

tempura_parser_rule_function_declaration:
	/* $1 */ TEMPURA_LEXER_TOKEN_KEYWORD_FUNCTION
	/* $2 */ tempura_parser_rule_function_name
	/* $3 */ TEMPURA_LEXER_TOKEN_PARENTHESIS_LEFT
	/* $4 */ tempura_parser_rule_parameter_list
	/* $5 */ TEMPURA_LEXER_TOKEN_PARENTHESIS_RIGHT
	/* $6 */ TEMPURA_LEXER_TOKEN_BRACE_LEFT
	/* $7 */ tempura_parser_rule_function_body
	/* $8 */ TEMPURA_LEXER_TOKEN_BRACE_RIGHT
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_FUNCTION_DECLARATION, 3, $2, $4, $7); };

tempura_parser_rule_import:
	/* $1 */ TEMPURA_LEXER_TOKEN_KEYWORD_IMPORT
	/* $2 */ tempura_parser_rule_expression
	/* $3 */ TEMPURA_LEXER_TOKEN_SEMICOLON
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_IMPORT, 1, $2); };

tempura_parser_rule_top_statement:
	/* $1 */ tempura_parser_rule_statement
	{ $$ = $1; } |
	/* $1 */ tempura_parser_rule_import
	{ $$ = $1; } |
	/* $1 */ tempura_parser_rule_function_declaration
	{ $$ = $1; };

tempura_parser_rule_top_statement_list:
	/* $1 */ tempura_parser_rule_top_statement_list
	/* $2 */ tempura_parser_rule_top_statement
	{ tempura_ast_node_append(parser->memory, $1, $2); $$ = $1; } |
	/* empty */
	{ tempura_ast_node_allocate(parser->memory, &$$, TEMPURA_AST_NODE_TYPE_STATEMENT_LIST, 0); };

%%

void tempura_parser_error_set(struct tempura_parser_context* parser, const char* message) {
	tempura_error_set(parser->error, TEMPURA_ERROR_TYPE_COMPILE_ERROR, message);
}

static inline void tempura_ast_node_print(uint8_t type, FILE* stream) {
	switch (type) {
		#define X(__type) case __type: fprintf(stream, #__type); break;
			TEMPURA_AST_NODE_TYPES_XMACRO
		#undef X
	}
}

static void tempura_ast_dump(uint16_t index, struct tempura_ast_node* node, int indent, FILE* stream);

static void tempura_ast_dump(uint16_t index, struct tempura_ast_node* node, int indent, FILE* stream) {
	char buffer[0xFF];
	memset(buffer, '\t', 0xFF);
	buffer[indent] = '\0';
	if (node == NULL) {
		fprintf(stream, "%s%" PRIu16 " => NULL;\n", buffer, index);
	} else {
		fprintf(stream, "%s%" PRIu16 " => ", buffer, index);
		tempura_ast_node_print(node->type, stream);
		switch (node->type) {
			case TEMPURA_AST_NODE_TYPE_HTML_ATTRIBUTE_NAME:
			case TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_CLASS:
			case TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_ID:
			case TEMPURA_AST_NODE_TYPE_IDENTIFIER:
			case TEMPURA_AST_NODE_TYPE_STRING_LITERAL:
			case TEMPURA_AST_NODE_TYPE_VARIABLE_NAME:
			case TEMPURA_AST_NODE_TYPE_VARIABLE_PROPERTY:
				fprintf(stream, ": \"%.*s\";\n", (int)(uint16_t)node->content.length, (char*)node->content.bytes);
				break;
			default:
				fprintf(stream, "[%" PRIu16 "]: {\n", (uint16_t)node->child_count);
				for (uint16_t i = 0; i < node->child_count; ++i) {
					tempura_ast_dump(i, node->children[i], indent + 1, stream);
				}
				fprintf(stream, "%s}\n", buffer);
				break;
		}
	}
}

enum tempura_result tempura_parser_test(char* buffer, size_t buffer_len) {
	enum tempura_result retval = TEMPURA_RESULT_FAILURE;
	struct tempura_memory_context memory;
	if (tempura_memory_initialize(&memory, 4096) == TEMPURA_RESULT_FAILURE) {
		fprintf(stderr, "tempura_memory_initialize() failed\n");
	} else {
		struct tempura_error error;
		struct tempura_lexer_context lexer;
		error.message.length = 255;
		tempura_memory_allocate(&memory, (void**)&error.message.bytes, error.message.length * sizeof (uint8_t));
		if (tempura_lexer_initialize(&lexer, &memory, buffer, buffer_len) == TEMPURA_RESULT_FAILURE) {
			fprintf(stderr, "tempura_lexer_initialize() failed\n");
		} else {
			struct tempura_parser_context parser;
			lexer.error = &error;
			parser.lexer = &lexer;
			parser.memory = &memory;
			parser.error = &error;
			if (tempura_parse(&parser) != 0) {
				fprintf(stderr, "Parse error: %.*s\n", (int)(uint16_t)parser.error->message.length, (char*)parser.error->message.bytes);
			} else {
				tempura_ast_dump(0, parser.ast, 0, stdout);
				retval = TEMPURA_RESULT_SUCCESS;
			}
			tempura_memory_free(&memory, (void**)&error.message.bytes, error.message.length * sizeof (uint8_t));
			tempura_lexer_destroy(&lexer);
		}
		tempura_memory_destroy(&memory);
	}
	return retval;
}
