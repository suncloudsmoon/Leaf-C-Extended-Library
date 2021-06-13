/*
Description: A simple system in which you can create a .url file by setting the attributes of a shortcut using the struct Shortcut descripted in urlcreator.h
Version: 5/29/2021
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

#include "urlcreator.h"

/**
Creates a URL shortcut according to the attributes in the Shortcut struct memory block.
*/
void leaf_createURLShortcut(leaf_shortcut *link) {
    FILE *shortcut = fopen(link->dest, "w");
    /*
    Internet shortcut example:
    [InternetShortcut]
    URL=http://www.google.com
    IconFile=http://www.google.com/favicon.ico
    IconIndex=0
    */
    fprintf(shortcut, "[InternetShortcut]\n");
    fprintf(shortcut, "URL=%s\n", link->url);
    fprintf(shortcut, "IconFile=%s\n", link->iconDir);
    fprintf(shortcut, "IconIndex=0\n");

    fprintf(shortcut, "[{5CBF2787-48CF-4208-B90E-EE5E5D420294}]\nProp21=31,%s\n", link->description); // Description
    fprintf(shortcut, "[{B9B4B3FC-2B51-4A42-B5D8-324146AFCF25}]\nProp5=31,%s\n", link->notes); // Notes
    fprintf(shortcut, "[{64440492-4C8B-11D1-8B70-080036B11A03}]\nProp9=19,%d\n", link->rating); // Rating

    fclose(shortcut);
}
