/*
Description: Has functions that simply string operations, like initializing a string with certain length and setting the memory inside the string to zero all in one function and more.
Version: 5/31/2021
Author(s): suncloudsmoon
Origin: N/A
License: MIT
*/

/*
 * Copyright (c) 2021, suncloudsmoon and the LeafGame2D contributors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "simplemalloc.h"
#include "stringbasics.h"

// length 6
#define VOWELS "aeiouy"
// length 19
#define CONSONANTS "bcdfghjklmnprstvwxz"


char* leaf_initString(int length) {
    /// TODO: Peplace this with calloc ///
    char *temp = leaf_charmalloc(length);
    memset(temp, 0, sizeof(temp)); // reset the array to zero
    return temp;
}

void leaf_memToZero(void* item) {
    memset(item, 0, sizeof(item));
}

/**
The length of the variable num must be greater than zero.
*/
char* leaf_generateWord(int num) {
    char *word = leaf_charmalloc(num);

    if (num > 0) {
        // The first letter
        int randChoice = rand() % 101;
        if (randChoice >= 0 && randChoice <= 65) {
            word[0] = CONSONANTS[rand() % 19];

        } else {
            word[0] = VOWELS[rand() % 6];
        }

        // The middle, where there should be more number of vowels than consonants
        for (int i = 1; i < num - 1; i++) {
            int secondChoice = rand() % 101;
            if (secondChoice >= 0 && secondChoice <= 62) {
                word[i] = VOWELS[rand() % 6];

            } else if (secondChoice >= 63 && secondChoice <= 95) {
                word[i] = CONSONANTS[rand() % 19];

            } else {
                word[i] = (char) (rand() % 26 + 97);
            }

        }

        // The last letter
        int thirdChoice = rand() % 101;
        if (thirdChoice >= 0 && thirdChoice <= 82) {
            word[num - 1] = CONSONANTS[rand() % 19];
        } else {
            word[num - 1] = VOWELS[rand() % 6];
        }
    }

    return word;
}

void leaf_addStrings(char *dest, char *one, char *two, char *three) {
    strcat(dest, one);
    strcat(dest, two);
    strcat(dest, three);
}
