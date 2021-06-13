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

#include "filemanager.h"

// Local Functions
static void addMemory(int num, char *input);
static int getFileLength(FILE *file);

void leaf_saveFile(char *location, char *data) {
    FILE *save = fopen(location, "w");
    int success = fputs(data, save);

    fclose(save);
}

char* leaf_retrieveFile(char *location) {
    char *output;
    FILE *inside = fopen(location, "r");

    if (inside) {
        // Get the length of the file
        int length = getFileLength(inside) + 1;
        output = (char *) malloc(length + 1 * sizeof(char));
        fgets(output, length, inside);
    }

    fclose(inside);

    return output;
}

static int getFileLength(FILE *file) {
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    return length;
}

static void addMemory(int num, char *input) {
    realloc(input, num * sizeof(char));
}
