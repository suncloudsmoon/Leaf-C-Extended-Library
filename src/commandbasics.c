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
#include <stdbool.h>
#include <string.h>
#include <windows.h>

#include "stringbasics.h"
#include "commandbasics.h"

static char** locations;
static int numLocations = 1000;
static int locationPos = 0;

char* leaf_getExePath() {
    // Getting the path of our exe
    char *ourPath = (char *) calloc(1000, sizeof(char));

    GetModuleFileName(NULL, ourPath, 1000);
    ourPath[strcspn(&ourPath, "\n")] = 0;

    return ourPath;
}

char* leaf_command(char *command) {
    char *fullCommand = leaf_initString(1000);
    char *output = leaf_initString(1000);

    leaf_memToZero(fullCommand);
    leaf_memToZero(output);

    strcat(fullCommand, "@echo off ");
    strcat(fullCommand, command);

    FILE *outputFile = popen(fullCommand, "r");

    // Initializing the temp string
    char *temp = leaf_initString(1000);
    leaf_memToZero(temp);

    fgets(temp, 1000, outputFile);
    temp[strcspn(temp, "\n")] = 0;
    strcat(output, temp);

    free(temp);

    return output;
}

bool leaf_sendToastNotification() {
    char command[1000];
    leaf_memToZero(&command);

    return leaf_command(&command) != NULL;
}

static char* malloctracker(int length) {

    if (locationPos < numLocations) {
        char *something = (char *) calloc(length, sizeof(char));

        locations[locationPos] = something;
        locationPos++;

        return something;
    } else {
        return NULL;
    }

}

void leaf_initCommandBasics() {
    locations = (char**) calloc(numLocations, sizeof(char*));
}

void leaf_freeCommandBasics() {
    for (int i = 0; i <= locationPos; i++) {
        free(locations[i]);
    }
    free(locations);
}
