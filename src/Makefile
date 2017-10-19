SHELL = /bin/bash

CFLAGS ?= -Werror -Wall -Wextra -Wpedantic -pedantic-errors -O3
LDFLAGS ?= -lc

LEX = re2c
LFLAGS = --no-generation-date

YACC = bison
YFLAGS = --feature=diagnostics-show-caret --report=none -Werror=all -Wno-empty-rule

OBJS = $(shell ls tempura*.{c,re,y} | tr . ' ' | awk '{print $$1".o"}' | sort | uniq)

all: tempura_lexer_tokens.h $(OBJS)

tempura_lexer_tokens.h: tempura_lexer.re
	@echo -e '#ifndef TEMPURA_LEXER_TOKENS_H\n#define TEMPURA_LEXER_TOKENS_H\n' > '$@'
	@echo -e '#define TEMPURA_LEXER_PSEUDOTOKENS_XMACRO \\' >> '$@'
	@grep -o 'return TEMPURA_LEXER_PSEUDOTOKEN_[A-Z_]*' '$<' | awk '{print "\tX("$$2") \\"}' | sort | uniq >> '$@'
	@echo -e '\n#define TEMPURA_LEXER_TOKENS_XMACRO \\' >> '$@'
	@grep -o 'return TEMPURA_LEXER_TOKEN_[A-Z_]*' '$<' | awk '{print "\tX("$$2") \\"}' | sort | uniq >> '$@'
	@echo -e '\n#endif' >> '$@'

tempura_lexer.c: tempura_lexer.re tempura_lexer_tokens.h
	$(LEX) $(LFLAGS) -b -c -i -o '$@' '$<'

tempura_parser.c: tempura_parser.y
	$(YACC) $(YFLAGS) --output='$@' '$<'

%.o: %.c
	$(CC) -std=c11 $(CFLAGS) -o '$@' -c '$<'

clean:
	rm -f *.o tempura_lexer.c tempura_lexer_tokens.h tempura_parser.c

.PHONY: all clean
