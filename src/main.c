/*
 * Copyright (c) 2021, suncloudsmoon and the Leaf C Extended Library contributors.
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

static void test_str();
static void test_list();

int main() {
	test_str();
	test_list();
	return 0;
}

static void test_str() {
	jtr_t *str = calloc(1, sizeof(jtr_t));
	jtrnew(str, "Hello ");
	jtrcat(str, "World!");

	// String stats
	printf("Str Content: %s, %zu, %zu\n", str->buf, str->length,
			str->allocated_length);

	jtr_t *newStr = malloc(sizeof(jtr_t));
	*newStr = (jtr_t ) { 0 };
	jtrsub(newStr, str, 0, 5);

	// After substring
	printf("After substring: %s, %zu, %zu\n", newStr->buf, newStr->length,
			newStr->allocated_length);

	jtrfree(str);
	jtrfree(newStr);
	free(str);
	free(newStr);
}

static void test_list() {
	jtrlist_t *list = malloc(sizeof(jtrlist_t));
	*list = (jtrlist_t ) { 0 };
	jtrlist_new(list);

	jtr_t *str1 = malloc(sizeof(jtr_t));
	*str1 = (jtr_t ) { 0 };

	jtrnew(str1, "Hello, World!");
	jtrlist_add(list, str1);

	printf("First item in the list: %s\n", jtrlist_get(list, 0)->buf);
	// List stats
	printf("List Stats: %zu, %zu", list->length, list->allocated_length);

	jtrfree(str1);
	jtrlist_free(list);
	free(str1);
	free(list);
}
