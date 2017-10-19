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

#include <stdarg.h> // for va_list, va_start(), va_end()
#include <stdint.h> // for uint8_t, uint16_t, uint_fast16_t
#include <stdio.h> // for vsnprintf()

#include "tempura_error.h"

#define TEMPURA_ERROR_MESSAGE_BUFFER_OVERFLOW "Buffer overflow"
#define TEMPURA_ERROR_MESSAGE_ENCODING_ERROR "Encoding error"

void tempura_error_set(struct tempura_error* error, enum tempura_error_type type, const char* format, ...) {
	va_list arguments;
	va_start(arguments, format);
	tempura_error_set_arguments(error, type, format, arguments);
	va_end(arguments);
}

void tempura_error_set_arguments(struct tempura_error* error, enum tempura_error_type type, const char* format, va_list arguments) {
	int length = vsnprintf((char*)error->message.bytes, error->message.length, format, arguments);
	if (length < 0) {
		error->message.bytes = (uint8_t*)TEMPURA_ERROR_MESSAGE_ENCODING_ERROR;
		error->message.length = sizeof TEMPURA_ERROR_MESSAGE_ENCODING_ERROR;
		error->type = TEMPURA_ERROR_TYPE_INTERNAL_ERROR;
	} else if (length > (int)(uint16_t)error->message.length) {
		error->message.bytes = (uint8_t*)TEMPURA_ERROR_MESSAGE_BUFFER_OVERFLOW;
		error->message.length = sizeof TEMPURA_ERROR_MESSAGE_BUFFER_OVERFLOW;
		error->type = TEMPURA_ERROR_TYPE_INTERNAL_ERROR;
	} else {
		error->message.length = (uint_fast16_t)(uint16_t)length;
		error->type = type;
	}
}
