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

#ifndef TEMPURA_ERROR_H
#define TEMPURA_ERROR_H

#include <stdarg.h> // for va_list
#include <stdint.h> // for UINT8_MAX

#include "tempura_types.h" // for struct tempura_string

enum tempura_error_type {
	TEMPURA_ERROR_TYPE_NONE = 0,
	TEMPURA_ERROR_TYPE_INVALID_ESCAPE_SEQUENCE = 0x10,
	TEMPURA_ERROR_TYPE_UNEXPECTED_CHARACTER,
	TEMPURA_ERROR_TYPE_UNTERMINATED_STRING,
	TEMPURA_ERROR_TYPE_UNTERMINATED_COMMENT,
	TEMPURA_ERROR_TYPE_COMPILE_ERROR = 0x20,
	TEMPURA_ERROR_TYPE_INTERNAL_ERROR = UINT8_MAX,
};

struct tempura_error {
	enum tempura_error_type type;
	struct tempura_string message;
};

void tempura_error_set(struct tempura_error* error, enum tempura_error_type type, const char* format, ...);
void tempura_error_set_arguments(struct tempura_error* error, enum tempura_error_type type, const char* format, va_list arguments);

#endif
