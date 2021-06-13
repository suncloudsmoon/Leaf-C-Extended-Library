/*
 * Description: Has functions that simply string operations, like initializing a string with certain length and setting the memory inside the string to zero all in one function and more.
 * Version: 5/29/2021
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

#ifndef NOOBENCRYPTION_H_INCLUDED
#define NOOBENCRYPTION_H_INCLUDED

// A very secure encryption and fast algorithm because it is not known to the world and it uses shift based encryption
char* leaf_encrypt(double key, char *data);
char* leaf_decrypt(double key, char *data);

void leaf_setCryptoSeed(long double seed);
int leaf_cryptorand();

#endif // NOOBENCRYPTION_H_INCLUDED
