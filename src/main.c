/*
 * Copyright (c) 2021, suncloudsmoon and the LeafGame2D contributors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * main.c
 *
 *  Created on: Sep 5, 2021
 *      Author: suncloudsmoon
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/leaflib/estr.h"

static void test_estr();

int main() {
	test_estr();
	return 0;
}

static void test_estr() {
	jtr_t *str = malloc(sizeof(jtr_t));
	str->optimization_level = 0.0;
	jtrnew(str, "Hello ");
	jtrcat(str, "World!");

	// String stats
	printf("Str Content: %s, %zu, %zu\n", str->text, str->length, str->allocated_length);

	jtr_t *newStr = malloc(sizeof(jtr_t));
	*newStr = (jtr_t ) { 0 };
	jtrsub(newStr, str, 0, 5);

	// After substring
	printf("After substring: %s, %zu, %zu\n", newStr->text, newStr->length, newStr->allocated_length);

}
