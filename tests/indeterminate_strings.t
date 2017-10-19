--TEMPLATE--
function str($input) {
	"prefix${input}suffix";
}

one {
	@two: str('three');
	str('four');
}

--TOKENS--
TEMPURA_LEXER_TOKEN_KEYWORD_FUNCTION
TEMPURA_LEXER_TOKEN_IDENTIFIER "str"
TEMPURA_LEXER_TOKEN_PARENTHESIS_LEFT
TEMPURA_LEXER_TOKEN_VARIABLE_NAME "input"
TEMPURA_LEXER_TOKEN_PARENTHESIS_RIGHT
TEMPURA_LEXER_TOKEN_BRACE_LEFT
TEMPURA_LEXER_TOKEN_DOUBLE_QUOTE
TEMPURA_LEXER_TOKEN_STRING_LITERAL "prefix"
TEMPURA_LEXER_TOKEN_VARIABLE_NAME "input"
TEMPURA_LEXER_TOKEN_STRING_LITERAL "suffix"
TEMPURA_LEXER_TOKEN_DOUBLE_QUOTE
TEMPURA_LEXER_TOKEN_SEMICOLON
TEMPURA_LEXER_TOKEN_BRACE_RIGHT
TEMPURA_LEXER_TOKEN_IDENTIFIER "one"
TEMPURA_LEXER_TOKEN_BRACE_LEFT
TEMPURA_LEXER_TOKEN_HTML_ATTRIBUTE_NAME "two"
TEMPURA_LEXER_TOKEN_COLON
TEMPURA_LEXER_TOKEN_IDENTIFIER "str"
TEMPURA_LEXER_TOKEN_PARENTHESIS_LEFT
TEMPURA_LEXER_TOKEN_STRING_LITERAL "three"
TEMPURA_LEXER_TOKEN_PARENTHESIS_RIGHT
TEMPURA_LEXER_TOKEN_SEMICOLON
TEMPURA_LEXER_TOKEN_IDENTIFIER "str"
TEMPURA_LEXER_TOKEN_PARENTHESIS_LEFT
TEMPURA_LEXER_TOKEN_STRING_LITERAL "four"
TEMPURA_LEXER_TOKEN_PARENTHESIS_RIGHT
TEMPURA_LEXER_TOKEN_SEMICOLON
TEMPURA_LEXER_TOKEN_BRACE_RIGHT
TEMPURA_LEXER_PSEUDOTOKEN_END

--AST--
0 => TEMPURA_AST_NODE_TYPE_STATEMENT_LIST[2]: {
	0 => TEMPURA_AST_NODE_TYPE_FUNCTION_DECLARATION[3]: {
		0 => TEMPURA_AST_NODE_TYPE_IDENTIFIER: "str";
		1 => TEMPURA_AST_NODE_TYPE_PARAMETER_LIST[1]: {
			0 => TEMPURA_AST_NODE_TYPE_PARAMETER[2]: {
				0 => TEMPURA_AST_NODE_TYPE_VARIABLE_NAME: "input";
				1 => NULL;
			}
		}
		2 => TEMPURA_AST_NODE_TYPE_STATEMENT_LIST[1]: {
			0 => TEMPURA_AST_NODE_TYPE_STRING_INTERPOLATION_LIST[3]: {
				0 => TEMPURA_AST_NODE_TYPE_STRING_LITERAL: "prefix";
				1 => TEMPURA_AST_NODE_TYPE_VARIABLE[2]: {
					0 => TEMPURA_AST_NODE_TYPE_VARIABLE_NAME: "input";
					1 => TEMPURA_AST_NODE_TYPE_VARIABLE_PROPERTY_LIST[0]: {
					}
				}
				2 => TEMPURA_AST_NODE_TYPE_STRING_LITERAL: "suffix";
			}
		}
	}
	1 => TEMPURA_AST_NODE_TYPE_HTML_ELEMENT[3]: {
		0 => TEMPURA_AST_NODE_TYPE_IDENTIFIER: "one";
		1 => TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_LIST[0]: {
		}
		2 => TEMPURA_AST_NODE_TYPE_STATEMENT_LIST[2]: {
			0 => TEMPURA_AST_NODE_TYPE_HTML_ATTRIBUTE[2]: {
				0 => TEMPURA_AST_NODE_TYPE_HTML_ATTRIBUTE_NAME: "two";
				1 => TEMPURA_AST_NODE_TYPE_FUNCTION_CALL[2]: {
					0 => TEMPURA_AST_NODE_TYPE_IDENTIFIER: "str";
					1 => TEMPURA_AST_NODE_TYPE_ARGUMENT_LIST[1]: {
						0 => TEMPURA_AST_NODE_TYPE_STRING_LITERAL: "three";
					}
				}
			}
			1 => TEMPURA_AST_NODE_TYPE_FUNCTION_CALL[2]: {
				0 => TEMPURA_AST_NODE_TYPE_IDENTIFIER: "str";
				1 => TEMPURA_AST_NODE_TYPE_ARGUMENT_LIST[1]: {
					0 => TEMPURA_AST_NODE_TYPE_STRING_LITERAL: "four";
				}
			}
		}
	}
}