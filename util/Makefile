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

CFLAGS ?= -Werror -Wall -Wextra -Wpedantic -pedantic-errors -O3
LDFLAGS ?= -lc

SRCDIR = ../src

all: tempura_lexer_test tempura_parser_test

tempura_lexer_test: \
	tempura_lexer_test.c \
		$(SRCDIR)/tempura_ast.o \
		$(SRCDIR)/tempura_error.o \
		$(SRCDIR)/tempura_lexer.o \
		$(SRCDIR)/tempura_memory.o
	$(CC) -I'${SRCDIR}' -std=c11 $(CFLAGS) $(LDFLAGS) -o '$@' '$<' \
		'$(SRCDIR)'/tempura_ast.o \
		'$(SRCDIR)'/tempura_error.o \
		'$(SRCDIR)'/tempura_lexer.o \
		'$(SRCDIR)'/tempura_memory.o

tempura_parser_test: \
	tempura_parser_test.c \
		$(SRCDIR)/tempura_ast.o \
		$(SRCDIR)/tempura_error.o \
		$(SRCDIR)/tempura_lexer.o \
		$(SRCDIR)/tempura_memory.o \
		$(SRCDIR)/tempura_parser.o
	$(CC) -I'$(SRCDIR)' -std=c11 $(CFLAGS) $(LDFLAGS) -o '$@' '$<' \
		'$(SRCDIR)'/tempura_ast.o \
		'$(SRCDIR)'/tempura_error.o \
		'$(SRCDIR)'/tempura_lexer.o \
		'$(SRCDIR)'/tempura_memory.o \
		'$(SRCDIR)'/tempura_parser.o \

clean:
	rm -f \
		tempura_lexer_test \
		tempura_parser_test \

.PHONY: all clean
