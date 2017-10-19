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

#ifndef TEMPURA_AST_H
#define TEMPURA_AST_H

#include <stdint.h> // for uint_fast16_t

#include "tempura_memory.h" // for struct tempura_memory_context
#include "tempura_types.h" // for enum tempura_result

#define TEMPURA_AST_NODE_TYPES_XMACRO \
	X(TEMPURA_AST_NODE_TYPE_ARGUMENT_LIST) \
	X(TEMPURA_AST_NODE_TYPE_FOREACH) \
	X(TEMPURA_AST_NODE_TYPE_FUNCTION_CALL) \
	X(TEMPURA_AST_NODE_TYPE_FUNCTION_DECLARATION) \
	X(TEMPURA_AST_NODE_TYPE_HTML_ATTRIBUTE) \
	X(TEMPURA_AST_NODE_TYPE_HTML_ATTRIBUTE_LIST) \
	X(TEMPURA_AST_NODE_TYPE_HTML_ATTRIBUTE_NAME) \
	X(TEMPURA_AST_NODE_TYPE_HTML_CHILD_LIST) \
	X(TEMPURA_AST_NODE_TYPE_HTML_ELEMENT) \
	X(TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_CLASS) \
	X(TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_ID) \
	X(TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_LIST) \
	X(TEMPURA_AST_NODE_TYPE_IDENTIFIER) \
	X(TEMPURA_AST_NODE_TYPE_IMPORT) \
	X(TEMPURA_AST_NODE_TYPE_PARAMETER) \
	X(TEMPURA_AST_NODE_TYPE_PARAMETER_LIST) \
	X(TEMPURA_AST_NODE_TYPE_STATEMENT_LIST) \
	X(TEMPURA_AST_NODE_TYPE_STRING_LITERAL) \
	X(TEMPURA_AST_NODE_TYPE_STRING_INTERPOLATION_LIST) \
	X(TEMPURA_AST_NODE_TYPE_VARIABLE) \
	X(TEMPURA_AST_NODE_TYPE_VARIABLE_NAME) \
	X(TEMPURA_AST_NODE_TYPE_VARIABLE_PROPERTY) \
	X(TEMPURA_AST_NODE_TYPE_VARIABLE_PROPERTY_LIST) \

enum tempura_ast_node_type {
	TEMPURA_AST_NODE_PSEUDOTYPE_NONE,
	#define X(__type) __type,
		TEMPURA_AST_NODE_TYPES_XMACRO
	#undef X
};

struct tempura_ast_node {
	enum tempura_ast_node_type type;
	union {
		struct {
			uint_fast16_t child_count;
			struct tempura_ast_node** children;
		};
		struct tempura_string content;
	};
};

union tempura_semantic_value {
	struct tempura_ast_node* ast;
};

enum tempura_result tempura_ast_node_allocate(struct tempura_memory_context* memory, struct tempura_ast_node** node, enum tempura_ast_node_type type, uint_fast16_t child_count, ...);
enum tempura_result tempura_ast_node_append(struct tempura_memory_context* memory, struct tempura_ast_node* parent, struct tempura_ast_node* child);
void tempura_ast_node_free(struct tempura_memory_context* memory, struct tempura_ast_node** node);

#endif
