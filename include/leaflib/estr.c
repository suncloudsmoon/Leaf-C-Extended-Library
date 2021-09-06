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
 * str.c
 *
 *  Created on: Sep 4, 2021
 *      Author: suncloudsmoon
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estr.h"

static int str_automatic_realloc(jtr_t *dest, size_t add_factor);
static int strlist_automatic_realloc(jtrlist_t *list, size_t add_factor);

// You need to manually allocate the struct before (stack or heap)
int jtrnew(jtr_t *dest, char *src) {
	size_t srcLength = strlen(src);
	dest->buf = malloc(srcLength);
	strncpy(dest->buf, src, srcLength + 1);
	dest->allocated_length = srcLength + 1;
	dest->length = srcLength;
	return dest->buf != NULL;
}

char* jtrcpy(jtr_t *dest, const char *src) {
	size_t srcLength = strlen(src);
	if (str_automatic_realloc(dest, srcLength) == -1) {
		return NULL;
	}
	dest->length = srcLength;
	return strncpy(dest->buf, src, srcLength + 1);
}

char* jtrcpy_s(jtr_t *dest, const jtr_t *src) {
	if (str_automatic_realloc(dest, src->length) == -1) {
		return NULL;
	}
	dest->length = src->length;
	return strncpy(dest->buf, src->buf, src->length + 1);
}

char* jtrcat(jtr_t *dest, const char *src) {
	size_t srcLength = strlen(src);
	if (str_automatic_realloc(dest, dest->length + srcLength) == -1) {
		return NULL;
	}
	dest->length += srcLength;
	return strncat(dest->buf, src, srcLength + 1);
}

char* jtrcat_s(jtr_t *dest, const jtr_t *src) {
	if (str_automatic_realloc(dest, dest->length + src->length) == -1) {
		return NULL;
	}
	dest->length += src->length;
	return strncat(dest->buf, src->buf, src->length + 1);
}

int jtrsub(jtr_t *dest, const jtr_t *src, size_t start, size_t end) {
	size_t add_factor = end - start;
	if (add_factor > src->allocated_length) {
		return -1;
	}
	if (str_automatic_realloc(dest, dest->length + add_factor) == -1) {
		return -1;
	}

	strncpy(dest->buf, src->buf + start, add_factor);
	dest->buf[add_factor] = '\0';
	dest->length += add_factor;
	return dest->buf != NULL;
}

void jtrcls(jtr_t *dest) {
	dest->buf[0] = '\0';
	dest->length = 0;
}

// You need to free the struct itself manually
void jtrfree(jtr_t *dest) {
	free(dest->buf);
}

// A basic string list implementation
int jtrlist_new(jtrlist_t *list) {
	list->ite = malloc(LIST_MINIMUM_STRINGS_TO_ALLOCATE * sizeof(jtr_t));
	list->allocated_length = LIST_MINIMUM_STRINGS_TO_ALLOCATE;
	list->length = 0;
	return list->ite != NULL;
}

int jtrlist_add(jtrlist_t *list, jtr_t *str) {
	// memory stuff should be handled with care!
	int mem_status = strlist_automatic_realloc(list, list->length + 1);
	list->ite[list->length] = str;
	list->length++;
	return mem_status;
}

// TODO: check if this works
void jtrlist_remove(jtrlist_t *list, size_t index) {
	size_t bytes = sizeof(jtr_t) * (list->allocated_length - index - 1);
	memmove(&list->ite[index], &list->ite[index + 1], bytes);
	list->length--;
}

jtr_t* jtrlist_get(jtrlist_t *list, size_t index) {
	if (index > list->length) {
		return NULL;
	}
	return list->ite[index];
}

void jtrlist_cls(jtrlist_t *list) {
	list->length = 0;
}

void jtrlist_free(jtrlist_t *list) {
	free(list->ite);
}

static int str_automatic_realloc(jtr_t *dest, size_t add_factor) {
	size_t total_add_math = (dest->length * dest->optimization_level)
			+ (add_factor + 1);
	if (total_add_math > dest->allocated_length) {
		char *mem_check = realloc(dest->buf, total_add_math);
		if (mem_check == NULL) {
			return -1;
		}
		dest->buf = mem_check;
		dest->allocated_length = total_add_math;
	}
	return 0;
}

static int strlist_automatic_realloc(jtrlist_t *list, size_t add_factor) {
	size_t total_add_math = (list->optimization_level * list->length)
			+ (add_factor);
	if (total_add_math > list->allocated_length) {
		jtr_t **mem_check = realloc(list->ite,
				total_add_math * sizeof(jtr_t));
		if (mem_check == NULL) {
			return -1;
		}
		list->ite = mem_check;
		list->allocated_length = total_add_math;
	}
	return 0;
}

// Extension to the C String Library

int strsub(char *dest, const char *src, size_t start, size_t end) {
	size_t add_factor = end - start;
	strncat(dest, src + start, add_factor);
	dest[add_factor] = '\0';
	return 0;
}
