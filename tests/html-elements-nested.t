--TEMPLATE--
one two three four;

--TOKENS--
TEMPURA_LEXER_TOKEN_IDENTIFIER "one"
TEMPURA_LEXER_TOKEN_IDENTIFIER "two"
TEMPURA_LEXER_TOKEN_IDENTIFIER "three"
TEMPURA_LEXER_TOKEN_IDENTIFIER "four"
TEMPURA_LEXER_TOKEN_SEMICOLON
TEMPURA_LEXER_PSEUDOTOKEN_END

--AST--
0 => TEMPURA_AST_NODE_TYPE_STATEMENT_LIST[1]: {
	0 => TEMPURA_AST_NODE_TYPE_HTML_ELEMENT[3]: {
		0 => TEMPURA_AST_NODE_TYPE_IDENTIFIER: "one";
		1 => TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_LIST[0]: {
		}
		2 => TEMPURA_AST_NODE_TYPE_STATEMENT_LIST[1]: {
			0 => TEMPURA_AST_NODE_TYPE_HTML_ELEMENT[3]: {
				0 => TEMPURA_AST_NODE_TYPE_IDENTIFIER: "two";
				1 => TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_LIST[0]: {
				}
				2 => TEMPURA_AST_NODE_TYPE_STATEMENT_LIST[1]: {
					0 => TEMPURA_AST_NODE_TYPE_HTML_ELEMENT[3]: {
						0 => TEMPURA_AST_NODE_TYPE_IDENTIFIER: "three";
						1 => TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_LIST[0]: {
						}
						2 => TEMPURA_AST_NODE_TYPE_STATEMENT_LIST[1]: {
							0 => TEMPURA_AST_NODE_TYPE_HTML_ELEMENT[3]: {
								0 => TEMPURA_AST_NODE_TYPE_IDENTIFIER: "four";
								1 => TEMPURA_AST_NODE_TYPE_HTML_SHORTHAND_ATTRIBUTE_LIST[0]: {
								}
								2 => TEMPURA_AST_NODE_TYPE_STATEMENT_LIST[0]: {
								}
							}
						}
					}
				}
			}
		}
	}
}
