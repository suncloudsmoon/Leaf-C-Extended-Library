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
 * str.h
 *
 *  Created on: Sep 4, 2021
 *      Author: suncloudsmoon
 */

#ifndef INCLUDE_LEAFLIB_ESTR_H_
#define INCLUDE_LEAFLIB_ESTR_H_

#define LIST_MINIMUM_STRINGS_TO_ALLOCATE 10

typedef struct {
	char *buf;
	size_t allocated_length;
	size_t length;
	float optimization_level;
} jtr_t;

typedef struct {
	jtr_t **ite;
	size_t allocated_length;
	size_t length;
	float optimization_level;
} jtrlist_t;

// Our Custom String Library

/*
 * Mallocs dest->text with strdup and sets the length and allocated length of jtr_t by using the strlen() of src.
 */
int jtrnew(jtr_t *dest, char *src);

char* jtrcpy(jtr_t *dest, const char *src);
char* jtrcpy_s(jtr_t *dest, const jtr_t *src);
char* jtrcat(jtr_t *dest, const char *src);
char* jtrcat_s(jtr_t *dest, const jtr_t *src);

jtr_t* jtrdup(char *src);
jtr_t* jtrdup_s(jtr_t *src);

/*
 * CAUTION: you need to zero out dest (memset, calloc, etc.) before you pass it in there
 */
int jtrsub(jtr_t *dest, const jtr_t *src, size_t start, size_t end);

/*
 * Sets the first character of dest->text to NULL terminator - '\0' and sets the length of jtr_t struct to 0.
 */
void jtrcls(jtr_t *dest);
/*
 * Frees dest->text and not the structure itself.
 * You need to free the structure itself manually by calling free() on it.
 */
void jtrfree(jtr_t *dest);

// A basic string list implementation
int jtrlist_new(jtrlist_t *list);
int jtrlist_add(jtrlist_t *list, jtr_t *str);
void jtrlist_remove(jtrlist_t *list, size_t index);
jtr_t* jtrlist_get(jtrlist_t *list, size_t index);
void jtrlist_free(jtrlist_t *list);

// Extension to the C String Library

void strsub(char *dest, const char *src, size_t start, size_t end);

#endif /* INCLUDE_LEAFLIB_ESTR_H_ */
