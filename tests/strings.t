--TEMPLATE--
'one'
"two"
"abc${}def"

--TOKENS--
TEMPURA_LEXER_TOKEN_STRING_LITERAL "one"
TEMPURA_LEXER_TOKEN_STRING_LITERAL "two"
TEMPURA_LEXER_TOKEN_DOUBLE_QUOTE
TEMPURA_LEXER_TOKEN_STRING_LITERAL "abc"
TEMPURA_LEXER_TOKEN_STRING_LITERAL "def"
TEMPURA_LEXER_TOKEN_DOUBLE_QUOTE
TEMPURA_LEXER_PSEUDOTOKEN_END

--AST--
