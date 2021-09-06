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
 * str.c
 *
 *  Created on: Sep 4, 2021
 *      Author: suncloudsmoon
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estr.h"

static int automatic_realloc(jtr_t *dest, size_t add_factor);

// You need to manually allocate the struct before (stack or heap)
int jtrnew(jtr_t *dest, char *src) {
	size_t srcLength = strlen(src);
	dest->text = strdup(src);
	dest->allocated_length = srcLength + 1;
	dest->length = srcLength;
	return dest->text != NULL;
}

char* jtrcpy(jtr_t *dest, char *src) {
	size_t srcLength = strlen(src);
	automatic_realloc(dest, srcLength);
	dest->length = srcLength;
	return strcpy(dest->text, src);
}

char* jtrcpy_s(jtr_t *dest, jtr_t *src) {
	automatic_realloc(dest, src->length);
	dest->length = src->length;
	return strcpy(dest->text, src->text);
}

char* jtrcat(jtr_t *dest, char *src) {
	size_t srcLength = strlen(src);
	automatic_realloc(dest, srcLength);
	dest->length += srcLength;
	return strcat(dest->text, src);
}

char* jtrcat_s(jtr_t *dest, jtr_t *src) {
	automatic_realloc(dest, src->length);
	dest->length += src->length;
	return strcat(dest->text, src->text);
}

int jtrsub(jtr_t *dest, jtr_t *src, size_t start, size_t end) {
	size_t add_factor = end - start;
	if (add_factor > src->allocated_length) {
		return -1;
	}
	automatic_realloc(dest, add_factor);

	strncpy(dest->text, src->text + start, add_factor);
	dest->text[add_factor] = '\0';
	dest->length += add_factor;
	return dest->text != NULL;
}

void jtrcls(jtr_t *dest) {
	dest->text[0] = '\0';
	dest->length = 0;
}

// You need to free the struct itself manually
void jtrfree(jtr_t *dest) {
	free(dest->text);
}

static int automatic_realloc(jtr_t *dest, size_t add_factor) {
	size_t total_add_math = (dest->length * dest->optimization_level) + (dest->length + add_factor + 1);
	if (total_add_math > dest->allocated_length) {
		char *mem_check = realloc(dest->text, total_add_math);
		if (mem_check == NULL) {
			return -1;
		}
		dest->text = mem_check;
		dest->allocated_length += total_add_math;
	}
	return 0;
}

// Extension to the C String Library

int strsub(char *dest, char *src, size_t start, size_t end) {
	if (start < 0) {
		return -1;
	}
	size_t add_factor = end - start;
	strncat(dest, src + start, add_factor);
	dest[add_factor] = '\0';
	return 0;
}
