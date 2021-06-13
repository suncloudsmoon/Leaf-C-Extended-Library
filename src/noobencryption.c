/*
 * Description: An implementation of the Perlin noise function by Ken Perlin in 1D and 2D
 * Version: 6/12/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
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
#include <string.h>
#include <math.h>
#include <limits.h>

#include "noobencryption.h"

static long double a = 37, c = 997, m = INT_MAX;
static long double randNum;

char* leaf_encrypt(double key, char *data) {
    printf("\n=============Encoding=============\n");

    int length = strlen(data); // without counting the null terminator
    char *output = (char *) calloc(length + 1, sizeof(char));

    leaf_setCryptoSeed(key + length); // change to key + length later

    for (int pos = 0; pos < length; pos++) {
        int ascii = (int) data[pos];
        int rand = leaf_cryptorand() % 95;
        // printf("[Debug] Random Number: %d\n", rand);

        if (ascii + rand > 126) {
            output[pos] = ((ascii + rand) - 126) + 32;
        } else {
            output[pos] = ascii + rand;
        }

        // Debug
        // printf("[Debug] Encoded Character: %d\n", output[pos]);


    }
    output[length] = '\0';

    // Debug
    printf("[Debug] Length of data string: %d\n", length);

    return output;
}

char* leaf_decrypt(double key, char *data) {
    printf("\n=============Decoding=============\n");

    int length = strlen(data); // without counting the null terminator
    char *output = (char *) calloc(length + 1, sizeof(char));

    leaf_setCryptoSeed(key + length);

    for (int pos = 0; pos < length; pos++) {
        int ascii = (int) data[pos];
        int rand = leaf_cryptorand() % 95;
        // printf("[Debug] Decoding Random Number: %d\n", rand);

        if (ascii - rand < 32) {
            // rand -= ascii - 32;
            output[pos] = 126 - (rand - (ascii - 32));

            // Debug
//            printf("[Debug] Ascii: %d\n", ascii);
//            printf("[Debug] ascii - 32: %d\n", ascii - 32);
//            printf("[Debug] Rand after minus: %d\n", rand);
        } else {
            output[pos] = ascii - rand;
        }

        // Debug
        // printf("[Debug] Character Decoded: %d\n", output[pos]);

    }
    output[length] = '\0';

    // Debug
    printf("[Debug] Length of data string: %d\n", length);

    return output;
}

void leaf_setCryptoSeed(long double seed) {
    printf("INT Max: %d\n", INT_MAX);
    randNum = seed;
}

int leaf_cryptorand() {
    randNum = fmodl(a * powl(randNum, a + (log2l(randNum) * sinl(randNum)) ) - logl(randNum) + c, m);
    return (int) randNum;
}
