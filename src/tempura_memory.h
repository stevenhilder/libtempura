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

#ifndef TEMPURA_MEMORY_H
#define TEMPURA_MEMORY_H

#include <stddef.h> // for size_t

#include "tempura_types.h" // for enum tempura_result

struct tempura_memory_context {
	size_t size;
	size_t peak;
};

enum tempura_result tempura_memory_initialize(struct tempura_memory_context* memory, size_t size);
enum tempura_result tempura_memory_allocate(struct tempura_memory_context* memory, void** pointer, size_t size);
enum tempura_result tempura_memory_reallocate(struct tempura_memory_context* memory, void** pointer, size_t size);
void tempura_memory_free(struct tempura_memory_context* memory, void** pointer, size_t size);
void tempura_memory_destroy(struct tempura_memory_context* memory);

#endif
