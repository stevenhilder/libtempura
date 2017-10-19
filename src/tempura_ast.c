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

#include <stdarg.h> // for va_list, va_start(), va_arg(), va_end()
#include <stdint.h> // for uint16_t

#include "tempura_ast.h"

#include "tempura_memory.h" // for tempura_memory_allocate(), tempura_memory_free()
#include "tempura_types.h" // for struct tempura_string

enum tempura_result tempura_ast_node_allocate(struct tempura_memory_context* memory, struct tempura_ast_node** node, enum tempura_ast_node_type type, uint_fast16_t child_count, ...) {
	va_list arguments;
	va_start(arguments, child_count);
	switch (type) {
		case TEMPURA_AST_NODE_TYPE_HTML_ATTRIBUTE_NAME:
		case TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_CLASS:
		case TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_ID:
		case TEMPURA_AST_NODE_TYPE_IDENTIFIER:
		case TEMPURA_AST_NODE_TYPE_STRING_LITERAL:
		case TEMPURA_AST_NODE_TYPE_VARIABLE_NAME:
		case TEMPURA_AST_NODE_TYPE_VARIABLE_PROPERTY: {
			struct tempura_string* content = va_arg(arguments, struct tempura_string*);
			tempura_memory_allocate(memory, (void**)node, sizeof (struct tempura_ast_node));
			(*node)->content.length = content->length;
			(*node)->content.bytes = content->bytes;
			break;
		}
		default:
			tempura_memory_allocate(memory, (void**)node, sizeof (struct tempura_ast_node));
			(*node)->child_count = (uint16_t)child_count;
			if ((*node)->child_count > 0) {
				tempura_memory_allocate(memory, (void**)&(*node)->children, (*node)->child_count * sizeof (struct tempura_ast_node*));
				for (uint_fast16_t i = 0; i < (*node)->child_count; ++i) {
					(*node)->children[i] = va_arg(arguments, struct tempura_ast_node*);
				}
			}
			break;
	}
	va_end(arguments);
	(*node)->type = type;
	return TEMPURA_RESULT_SUCCESS;
}

enum tempura_result tempura_ast_node_append(struct tempura_memory_context* memory, struct tempura_ast_node* parent, struct tempura_ast_node* child) {
	tempura_memory_reallocate(memory, (void**)&parent->children, ++parent->child_count * sizeof (struct tempura_ast_node*));
	parent->children[parent->child_count - 1] = child;
	return TEMPURA_RESULT_SUCCESS;
}

void tempura_ast_node_free(struct tempura_memory_context* memory, struct tempura_ast_node** node) {
	if (*node != NULL) {
		switch ((*node)->type) {
			case TEMPURA_AST_NODE_TYPE_HTML_ATTRIBUTE_NAME:
			case TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_CLASS:
			case TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_ID:
			case TEMPURA_AST_NODE_TYPE_IDENTIFIER:
			case TEMPURA_AST_NODE_TYPE_STRING_LITERAL:
			case TEMPURA_AST_NODE_TYPE_VARIABLE:
			case TEMPURA_AST_NODE_TYPE_VARIABLE_PROPERTY:
				tempura_memory_free(memory, (void**)node, sizeof (struct tempura_ast_node));
				break;
			default:
				for (uint_fast16_t i = 0; i < (*node)->child_count; i++) {
					tempura_ast_node_free(memory, &(*node)->children[i]);
				}
				tempura_memory_free(memory, (void**)node, sizeof (struct tempura_ast_node) - sizeof (struct tempura_ast_node*) + ((*node)->child_count * sizeof (struct tempura_ast_node*)));
				break;
		}
	}
}
