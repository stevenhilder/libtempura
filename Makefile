#
# libtempura - HTML templating engine
#
# Written in 2017 by Steven Hilder <steven.hilder@sevenpercent.solutions>
#
# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication along with
# this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
#

SHELL = /bin/bash

SRCDIR = src
UTILDIR = util

ifeq ($(shell type sha1sum 2>/dev/null 1>&2; echo $$?),0)
SHA1SUM = sha1sum
else ifeq ($(shell type shasum 2>/dev/null 1>&2; echo $$?),0)
SHA1SUM = shasum
else
$(error SHA1 command not found)
endif

all:
	$(MAKE) -C '$(SRCDIR)'
	$(MAKE) -C '$(UTILDIR)'

clean:
	$(MAKE) -C '$(SRCDIR)' '$@'
	$(MAKE) -C '$(UTILDIR)' '$@'

test:
	@$(MAKE) --no-print-directory -C '$(SRCDIR)' 'tempura_ast.o' > /dev/null
	@$(MAKE) --no-print-directory -C '$(SRCDIR)' 'tempura_error.o' > /dev/null
	@$(MAKE) --no-print-directory -C '$(SRCDIR)' 'tempura_lexer.o' > /dev/null
	@$(MAKE) --no-print-directory -C '$(SRCDIR)' 'tempura_memory.o' > /dev/null
	@$(MAKE) --no-print-directory -C '$(SRCDIR)' 'tempura_parser.o' > /dev/null
	@$(MAKE) --no-print-directory -C '$(UTILDIR)' > /dev/null
	@echo; echo 'Lexer tests:'; \
	for f in tests/*.t; do \
		echo -en "\t[    ] $$f"; \
		template=$$(sed -n '/--TEMPLATE--/=' "$$f"); \
		tokens=$$(sed -n '/--TOKENS--/=' "$$f"); \
		ast=$$(sed -n '/--AST--/=' "$$f"); \
		tokens_expected=$$( \
			sed -n "$$(($$tokens + 1)),$$(($$ast - 1))p" "$$f" \
			| grep -v '^$$' \
			| $(SHA1SUM) \
		); \
		tokens_actual=$$( \
			sed -n "$$(($$template + 1)),$$(($$tokens - 1))p" "$$f" \
			| $(UTILDIR)/tempura_lexer_test 2>/dev/null \
			| $(SHA1SUM) \
		); \
		if [ "$$tokens_actual" = "$$tokens_expected" ]; then \
			echo -e '\r\t[PASS'; \
		else \
			echo -e '\r\t[FAIL'; \
		fi; \
	done
	@echo; echo 'Parser tests:'; \
	for f in tests/*.t; do \
		echo -en "\t[    ] $$f"; \
		template=$$(sed -n '/--TEMPLATE--/=' "$$f"); \
		tokens=$$(sed -n '/--TOKENS--/=' "$$f"); \
		ast=$$(sed -n '/--AST--/=' "$$f"); \
		ast_expected=$$( \
			tail -n +"$$(($$ast + 1))" "$$f" \
			| grep -v '^$$' \
			| $(SHA1SUM) \
		); \
		ast_actual=$$( \
			sed -n "$$(($$template + 1)),$$(($$tokens - 1))p" "$$f" \
			| $(UTILDIR)/tempura_parser_test 2>/dev/null \
			| $(SHA1SUM) \
		); \
		if [ "$$ast_actual" = "$$ast_expected" ]; then \
			echo -e '\r\t[PASS'; \
		else \
			echo -e '\r\t[FAIL'; \
		fi; \
	done

.PHONY: all clean test
