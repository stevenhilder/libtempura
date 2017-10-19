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

#include <stdlib.h> // for malloc(), realloc(), free()

#include "tempura_memory.h"

enum tempura_result tempura_memory_initialize(struct tempura_memory_context* memory, size_t size) {
	memory->size = size;
	memory->peak = memory->size;
	return TEMPURA_RESULT_SUCCESS;
}

enum tempura_result tempura_memory_allocate(struct tempura_memory_context* memory, void** pointer, size_t size) {
	memory->size += size;
	if (memory->size > memory->peak) {
		memory->peak = memory->size;
	}
	return (*pointer = malloc(size)) == NULL ? TEMPURA_RESULT_FAILURE : TEMPURA_RESULT_SUCCESS;
}

enum tempura_result tempura_memory_reallocate(struct tempura_memory_context* memory, void** pointer, size_t size) {
#	pragma message "Unused parameter 'memory' in tempura_memory_reallocate()"
	(void)memory;
	return (*pointer = realloc(*pointer, size)) == NULL ? TEMPURA_RESULT_FAILURE : TEMPURA_RESULT_SUCCESS;
}

void tempura_memory_free(struct tempura_memory_context* memory, void** pointer, size_t size) {
	memory->size -= size;
	free(*pointer);
	*pointer = NULL;
}

void tempura_memory_destroy(struct tempura_memory_context* memory) {
	memory->size = 0;
}
