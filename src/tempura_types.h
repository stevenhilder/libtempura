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

#ifndef TEMPURA_TYPES_H
#define TEMPURA_TYPES_H

#include <stdint.h> // for uint_fast16_t, uint8_t

enum tempura_result {
	TEMPURA_RESULT_SUCCESS = 0,
	TEMPURA_RESULT_FAILURE = 1,
};

struct tempura_string {
	uint_fast16_t length;
	uint8_t* bytes;
};

#endif
