/*
 * Description:
 * Version: 0.1
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT & Others
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

#include <math.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <windows.h>
#include <memory.h>


#include "leafgame2d.h"

////////////////////////////////////// START OF BRAIN.C ////////////////////////////////////////////////////////////////////////////////////

/*
 * Description: A simple neural network library that follows the basic neural network structure
 * Version: [Origin: 1.0], [Modified: 1.1]
 * Author(s): Lewis Van Winkle<LV@codeplea.com>, suncloudsmoon
 * Origin: https://github.com/codeplea/genann/blob/master/genann.c
 * License: Zlib
 */

/*
 * Zlib License Disclaimer:
 * [Terms]
 * The term "software" used in this context is used to refer to the "GENANN - Minimal C Artificial Neural Network" software.
 * The term "we" refers to potential future developers working on this modified source version.
 *
 * [Explanation]
 * This is an altered source version of the software and is substantially different from the original software.
 * Also, we did not write the original software and this is not the original software.
 */

/*
 * GENANN - Minimal C Artificial Neural Network
 *
 * Copyright (c) 2015-2018 Lewis Van Winkle
 *
 * http://CodePlea.com
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgement in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */

#ifndef leaf_brain_act
#define leaf_brain_act_hidden leaf_brain_act_hidden_indirect
#define leaf_brain_act_output leaf_brain_act_output_indirect
#else
#define leaf_brain_act_hidden leaf_brain_act
#define leaf_brain_act_output leaf_brain_act
#endif

#define LOOKUP_SIZE 4096

double leaf_brain_act_hidden_indirect(const struct leaf_brain *ann, double a) {
    return ann->activation_hidden(ann, a);
}

double leaf_brain_act_output_indirect(const struct leaf_brain *ann, double a) {
    return ann->activation_output(ann, a);
}

const double sigmoid_dom_min = -15.0;
const double sigmoid_dom_max = 15.0;
double interval;
double lookup[LOOKUP_SIZE];

#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#define unused          __attribute__((unused))
#else
#define likely(x)       x
#define unlikely(x)     x
#define unused
#pragma warning(disable : 4996) /* For fscanf */
#endif

double leaf_brain_act_sigmoid(const leaf_brain *ann unused, double a) {
    if (a < -45.0) return 0;
    if (a > 45.0) return 1;
    return 1.0 / (1 + exp(-a));
}

void leaf_brain_init_sigmoid_lookup(const leaf_brain *ann) {
    const double f = (sigmoid_dom_max - sigmoid_dom_min) / LOOKUP_SIZE;
    int i;

    interval = LOOKUP_SIZE / (sigmoid_dom_max - sigmoid_dom_min);
    for (i = 0; i < LOOKUP_SIZE; ++i) {
        lookup[i] = leaf_brain_act_sigmoid(ann, sigmoid_dom_min + f * i);
    }
}

double leaf_brain_act_sigmoid_cached(const leaf_brain *ann unused, double a) {
    assert(!isnan(a));

    if (a < sigmoid_dom_min) return lookup[0];
    if (a >= sigmoid_dom_max) return lookup[LOOKUP_SIZE - 1];

    size_t j = (size_t)((a-sigmoid_dom_min)*interval+0.5);

    /* Because floating point... */
    if (unlikely(j >= LOOKUP_SIZE)) return lookup[LOOKUP_SIZE - 1];

    return lookup[j];
}

double leaf_brain_act_linear(const struct leaf_brain *ann unused, double a) {
    return a;
}

double leaf_brain_act_threshold(const struct leaf_brain *ann unused, double a) {
    return a > 0;
}

leaf_brain *leaf_brain_init(int inputs, int hidden_layers, int hidden, int outputs) {
    if (hidden_layers < 0) return 0;
    if (inputs < 1) return 0;
    if (outputs < 1) return 0;
    if (hidden_layers > 0 && hidden < 1) return 0;


    const int hidden_weights = hidden_layers ? (inputs+1) * hidden + (hidden_layers-1) * (hidden+1) * hidden : 0;
    const int output_weights = (hidden_layers ? (hidden+1) : (inputs+1)) * outputs;
    const int total_weights = (hidden_weights + output_weights);

    const int total_neurons = (inputs + hidden * hidden_layers + outputs);

    /* Allocate extra size for weights, outputs, and deltas. */
    const int size = sizeof(leaf_brain) + sizeof(double) * (total_weights + total_neurons + (total_neurons - inputs));
    leaf_brain *ret = malloc(size);
    if (!ret) return 0;

    ret->inputs = inputs;
    ret->hidden_layers = hidden_layers;
    ret->hidden = hidden;
    ret->outputs = outputs;

    ret->total_weights = total_weights;
    ret->total_neurons = total_neurons;

    /* Set pointers. */
    ret->weight = (double*)((char*)ret + sizeof(leaf_brain));
    ret->output = ret->weight + ret->total_weights;
    ret->delta = ret->output + ret->total_neurons;

    leaf_brain_randomize(ret);

    ret->activation_hidden = leaf_brain_act_sigmoid_cached;
    ret->activation_output = leaf_brain_act_sigmoid_cached;

    leaf_brain_init_sigmoid_lookup(ret);

    return ret;
}


leaf_brain *leaf_brain_read(FILE *in) {
    int inputs, hidden_layers, hidden, outputs;
    int rc;

    errno = 0;
    rc = fscanf(in, "%d %d %d %d", &inputs, &hidden_layers, &hidden, &outputs);
    if (rc < 4 || errno != 0) {
        perror("fscanf");
        return NULL;
    }

    leaf_brain *ann = leaf_brain_init(inputs, hidden_layers, hidden, outputs);

    int i;
    for (i = 0; i < ann->total_weights; ++i) {
        errno = 0;
        rc = fscanf(in, " %le", ann->weight + i);
        if (rc < 1 || errno != 0) {
            perror("fscanf");
            leaf_brain_free(ann);

            return NULL;
        }
    }

    return ann;
}


leaf_brain *leaf_brain_copy(leaf_brain const *ann) {
    const int size = sizeof(leaf_brain) + sizeof(double) * (ann->total_weights + ann->total_neurons + (ann->total_neurons - ann->inputs));
    leaf_brain *ret = malloc(size);
    if (!ret) return 0;

    memcpy(ret, ann, size);

    /* Set pointers. */
    ret->weight = (double*)((char*)ret + sizeof(leaf_brain));
    ret->output = ret->weight + ret->total_weights;
    ret->delta = ret->output + ret->total_neurons;

    return ret;
}


void leaf_brain_randomize(leaf_brain *ann) {
    int i;
    for (i = 0; i < ann->total_weights; ++i) {
        double r = leaf_brain_RANDOM();
        /* Sets weights from -0.5 to 0.5. */
        ann->weight[i] = r - 0.5;
    }
}


void leaf_brain_free(leaf_brain *ann) {
    /* The weight, output, and delta pointers go to the same buffer. */
    free(ann);
}


double const *leaf_brain_run(leaf_brain const *ann, double const *inputs) {
    double const *w = ann->weight;
    double *o = ann->output + ann->inputs;
    double const *i = ann->output;

    /* Copy the inputs to the scratch area, where we also store each neuron's
     * output, for consistency. This way the first layer isn't a special case. */
    memcpy(ann->output, inputs, sizeof(double) * ann->inputs);

    int h, j, k;

    if (!ann->hidden_layers) {
        double *ret = o;
        for (j = 0; j < ann->outputs; ++j) {
            double sum = *w++ * -1.0;
            for (k = 0; k < ann->inputs; ++k) {
                sum += *w++ * i[k];
            }
            *o++ = leaf_brain_act_output(ann, sum);
        }

        return ret;
    }

    /* Figure input layer */
    for (j = 0; j < ann->hidden; ++j) {
        double sum = *w++ * -1.0;
        for (k = 0; k < ann->inputs; ++k) {
            sum += *w++ * i[k];
        }
        *o++ = leaf_brain_act_hidden(ann, sum);
    }

    i += ann->inputs;

    /* Figure hidden layers, if any. */
    for (h = 1; h < ann->hidden_layers; ++h) {
        for (j = 0; j < ann->hidden; ++j) {
            double sum = *w++ * -1.0;
            for (k = 0; k < ann->hidden; ++k) {
                sum += *w++ * i[k];
            }
            *o++ = leaf_brain_act_hidden(ann, sum);
        }

        i += ann->hidden;
    }

    double const *ret = o;

    /* Figure output layer. */
    for (j = 0; j < ann->outputs; ++j) {
        double sum = *w++ * -1.0;
        for (k = 0; k < ann->hidden; ++k) {
            sum += *w++ * i[k];
        }
        *o++ = leaf_brain_act_output(ann, sum);
    }

    /* Sanity check that we used all weights and wrote all outputs. */
    assert(w - ann->weight == ann->total_weights);
    assert(o - ann->output == ann->total_neurons);

    return ret;
}


void leaf_brain_train(leaf_brain const *ann, double const *inputs, double const *desired_outputs, double learning_rate) {
    /* To begin with, we must run the network forward. */
    leaf_brain_run(ann, inputs);

    int h, j, k;

    /* First set the output layer deltas. */
    {
        double const *o = ann->output + ann->inputs + ann->hidden * ann->hidden_layers; /* First output. */
        double *d = ann->delta + ann->hidden * ann->hidden_layers; /* First delta. */
        double const *t = desired_outputs; /* First desired output. */


        /* Set output layer deltas. */
        if (leaf_brain_act_output == leaf_brain_act_linear ||
                ann->activation_output == leaf_brain_act_linear) {
            for (j = 0; j < ann->outputs; ++j) {
                *d++ = *t++ - *o++;
            }
        } else {
            for (j = 0; j < ann->outputs; ++j) {
                *d++ = (*t - *o) * *o * (1.0 - *o);
                ++o;
                ++t;
            }
        }
    }


    /* Set hidden layer deltas, start on last layer and work backwards. */
    /* Note that loop is skipped in the case of hidden_layers == 0. */
    for (h = ann->hidden_layers - 1; h >= 0; --h) {

        /* Find first output and delta in this layer. */
        double const *o = ann->output + ann->inputs + (h * ann->hidden);
        double *d = ann->delta + (h * ann->hidden);

        /* Find first delta in following layer (which may be hidden or output). */
        double const * const dd = ann->delta + ((h+1) * ann->hidden);

        /* Find first weight in following layer (which may be hidden or output). */
        double const * const ww = ann->weight + ((ann->inputs+1) * ann->hidden) + ((ann->hidden+1) * ann->hidden * (h));

        for (j = 0; j < ann->hidden; ++j) {

            double delta = 0;

            for (k = 0; k < (h == ann->hidden_layers-1 ? ann->outputs : ann->hidden); ++k) {
                const double forward_delta = dd[k];
                const int windex = k * (ann->hidden + 1) + (j + 1);
                const double forward_weight = ww[windex];
                delta += forward_delta * forward_weight;
            }

            *d = *o * (1.0-*o) * delta;
            ++d;
            ++o;
        }
    }


    /* Train the outputs. */
    {
        /* Find first output delta. */
        double const *d = ann->delta + ann->hidden * ann->hidden_layers; /* First output delta. */

        /* Find first weight to first output delta. */
        double *w = ann->weight + (ann->hidden_layers
                                   ? ((ann->inputs+1) * ann->hidden + (ann->hidden+1) * ann->hidden * (ann->hidden_layers-1))
                                   : (0));

        /* Find first output in previous layer. */
        double const * const i = ann->output + (ann->hidden_layers
                                                ? (ann->inputs + (ann->hidden) * (ann->hidden_layers-1))
                                                : 0);

        /* Set output layer weights. */
        for (j = 0; j < ann->outputs; ++j) {
            *w++ += *d * learning_rate * -1.0;
            for (k = 1; k < (ann->hidden_layers ? ann->hidden : ann->inputs) + 1; ++k) {
                *w++ += *d * learning_rate * i[k-1];
            }

            ++d;
        }

        assert(w - ann->weight == ann->total_weights);
    }


    /* Train the hidden layers. */
    for (h = ann->hidden_layers - 1; h >= 0; --h) {

        /* Find first delta in this layer. */
        double const *d = ann->delta + (h * ann->hidden);

        /* Find first input to this layer. */
        double const *i = ann->output + (h
                                         ? (ann->inputs + ann->hidden * (h-1))
                                         : 0);

        /* Find first weight to this layer. */
        double *w = ann->weight + (h
                                   ? ((ann->inputs+1) * ann->hidden + (ann->hidden+1) * (ann->hidden) * (h-1))
                                   : 0);


        for (j = 0; j < ann->hidden; ++j) {
            *w++ += *d * learning_rate * -1.0;
            for (k = 1; k < (h == 0 ? ann->inputs : ann->hidden) + 1; ++k) {
                *w++ += *d * learning_rate * i[k-1];
            }
            ++d;
        }

    }

}


void leaf_brain_write(leaf_brain const *ann, FILE *out) {
    fprintf(out, "%d %d %d %d", ann->inputs, ann->hidden_layers, ann->hidden, ann->outputs);

    int i;
    for (i = 0; i < ann->total_weights; ++i) {
        fprintf(out, " %.20e", ann->weight[i]);
    }
}


////////////////////// END OF BRAIN.C ////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////// START OF COMMANDBASICS.C ////////////////////////////////////////////////////
/*
 * Description: An implementation of the Perlin noise function by Ken Perlin in 1D and 2D
 * Version: 6/12/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
 */

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
///////////////////////////////////////////// END OF COMMANDBASICS.C ////////////////////////////////////////////////////////////////

///////////////////////////////////////////// START OF FILEMANAGER.C ///////////////////////////////////////////////////////////////
/*
 * Description: An implementation of the Perlin noise function by Ken Perlin in 1D and 2D
 * Version: 6/12/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
 */

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
/////////////////////////////////////////////// END OF FILEMANAGER.C /////////////////////////////////////////////////////////

//////////////////////////////////////////////// START OF LICENSEMANAGER.C /////////////////////////////////////////////////
/*
 * Description: An implementation of the Perlin noise function by Ken Perlin in 1D and 2D
 * Version: 6/12/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
 */


//////////////////////////////////////////////////// END OF LICENSEMANAGER.C //////////////////////////////////////////////////

////////////////////////////////////////////// START OF NOOBENCRYPTION.C //////////////////////////////////////////////////////
/*
 * Description: An implementation of the Perlin noise function by Ken Perlin in 1D and 2D
 * Version: 6/12/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
 */

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
    }
    output[length] = '\0';

    // Debug
    // printf("[Debug] Length of data string: %d\n", length);

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
///////////////////////////////////////// END OF NOOBENCRYPTION.C //////////////////////////////////////////////////

///////////////////////////////////////// START OF PERLIN.C /////////////////////////////////////////////////
/*
 * Description: An implementation of the Perlin noise function by Ken Perlin in 1D and 2D
 * Version: [Origin: Jun 8, 2016], [Modified: April 19, 2021]
 * Author(s): Stefan Gustavson<stefan.gustavson@liu.se>, suncloudsmoon
 * Origin: https://github.com/stegu/perlin-noise
 * License: Public Domain
 */

//////////////////////// Original Author Stuff [Start] /////////////////////////////////////////////

// noise1234
//
// Author: Stefan Gustavson, 2003-2005
// Contact: stefan.gustavson@liu.se
//
// This code was GPL licensed until February 2011.
// As the original author of this code, I hereby
// release it into the public domain.
// Please feel free to use it for whatever you want.
// Credit is appreciated where appropriate, and I also
// appreciate being told where this code finds any use,
// but you may do as you like.

/*
 * This implementation is "Improved Noise" as presented by
 * Ken Perlin at Siggraph 2002. The 3D function is a direct port
 * of his Java reference code which was once publicly available
 * on www.noisemachine.com (although I cleaned it up, made it
 * faster and made the code more readable), but the 1D, 2D and
 * 4D functions were implemented from scratch by me.
 *
 * This is a backport to C of my improved noise class in C++
 * which was included in the Aqsis renderer project.
 * It is highly reusable without source code modifications.
 *
 */

//////////////////////// Original Author Stuff [End] /////////////////////////////////////////////

/// Header Include Stuff ///

// This is the new and improved, C(2) continuous interpolant
#define FADE(t) ( t * t * t * ( t * ( t * 6 - 15 ) + 10 ) )

#define FASTFLOOR(x) ( ((int)(x)<(x)) ? ((int)x) : ((int)x-1 ) )
#define LERP(t, a, b) ((a) + (t)*((b)-(a)))

// Prototyping Static Functions
static float grad1( int hash, float x );
static float grad2( int hash, float x, float y );
static float noise1( float x );
static float noise2( float x, float y );
static float pnoise1( float x, int px );
static float pnoise2( float x, float y, int px, int py );

//---------------------------------------------------------------------
// Static data

/*
 * Permutation table. This is just a random jumble of all numbers 0-255,
 * repeated twice to avoid wrapping the index at 255 for each lookup.
 * This needs to be exactly the same for all instances on all platforms,
 * so it's easiest to just keep it as static explicit data.
 * This also removes the need for any initialisation of this class.
 *
 * Note that making this an int[] instead of a char[] might make the
 * code run faster on platforms with a high penalty for unaligned single
 * byte addressing. Intel x86 is generally single-byte-friendly, but
 * some other CPUs are faster with 4-aligned reads.
 * However, a char[] is smaller, which avoids cache trashing, and that
 * is probably the most important aspect on most architectures.
 * This array is accessed a *lot* by the noise functions.
 * A vector-valued noise over 3D accesses it 96 times, and a
 * float-valued 4D noise 64 times. We want this to fit in the cache!
 */
unsigned char perm[] = {151,160,137,91,90,15,
                        131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
                        190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
                        88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
                        77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
                        102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
                        135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
                        5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
                        223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
                        129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
                        251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
                        49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
                        138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
                        151,160,137,91,90,15,
                        131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
                        190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
                        88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
                        77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
                        102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
                        135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
                        5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
                        223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
                        129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
                        251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
                        49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
                        138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
                       };

static float pSeed; // Perlin Seed

float leaf_generate1DPerlinNoise(float x) {
    // Debug
//    printf("Input: %f\n", x);

    return noise1(x - pSeed);
}

float leaf_generate2DPerlinNoise(float x, float y) {
    // Debug
//    printf("Input: %f, %f\n", x, y);

    return noise2(x - pSeed, y - pSeed);
}

void leaf_setSeed(int seed) {
    pSeed = (rand() % 100) / (float) 100;
}

//---------------------------------------------------------------------

/*
 * Helper functions to compute gradients-dot-residualvectors (1D to 2D)
 * Note that these generate gradients of more than unit length. To make
 * a close match with the value range of classic Perlin noise, the final
 * noise values need to be rescaled. To match the RenderMan noise in a
 * statistical sense, the approximate scaling values (empirically
 * determined from test renderings) are:
 * 1D noise needs rescaling with 0.188
 * 2D noise needs rescaling with 0.507
 * 3D noise needs rescaling with 0.936
 * 4D noise needs rescaling with 0.87
 * Note that these noise functions are the most practical and useful
 * signed version of Perlin noise. To return values according to the
 * RenderMan specification from the SL noise() and pnoise() functions,
 * the noise values need to be scaled and offset to [0,1], like this:
 * float SLnoise = (noise3(x,y,z) + 1.0) * 0.5;
 */
static float grad1( int hash, float x ) {
    int h = hash & 15;
    float grad = 1.0 + (h & 7);  // Gradient value 1.0, 2.0, ..., 8.0
    if (h&8) grad = -grad;         // and a random sign for the gradient
    return ( grad * x );           // Multiply the gradient with the distance
}

static float grad2( int hash, float x, float y ) {
    int h = hash & 7;      // Convert low 3 bits of hash code
    float u = h<4 ? x : y;  // into 8 simple gradient directions,
    float v = h<4 ? y : x;  // and compute the dot product with (x,y).
    return ((h&1)? -u : u) + ((h&2)? -2.0*v : 2.0*v);
}

//---------------------------------------------------------------------
/** 1D float Perlin noise, SL "noise()"
 */
static float noise1( float x ) {
    int ix0, ix1;
    float fx0, fx1;
    float s, n0, n1;

    ix0 = FASTFLOOR( x ); // Integer part of x
    fx0 = x - ix0;       // Fractional part of x
    fx1 = fx0 - 1.0f;
    ix1 = ( ix0+1 ) & 0xff;
    ix0 = ix0 & 0xff;    // Wrap to 0..255

    s = FADE( fx0 );

    n0 = grad1( perm[ ix0 ], fx0 );
    n1 = grad1( perm[ ix1 ], fx1 );

    // Debug Stuff...
    printf("Noise1 Return Call: %f\n", 0.188f * ( LERP( s, n0, n1 ) ));
    printf("n0: %f\n", n0);
    printf("n1: %f\n", n1);

    return 0.188f * ( LERP( s, n0, n1 ) );
}

//---------------------------------------------------------------------
/** 1D float Perlin periodic noise, SL "pnoise()"
 */
static float pnoise1( float x, int px ) {
    int ix0, ix1;
    float fx0, fx1;
    float s, n0, n1;

    ix0 = FASTFLOOR( x ); // Integer part of x
    fx0 = x - ix0;       // Fractional part of x
    fx1 = fx0 - 1.0f;
    ix1 = (( ix0 + 1 ) % px) & 0xff; // Wrap to 0..px-1 *and* wrap to 0..255
    ix0 = ( ix0 % px ) & 0xff;      // (because px might be greater than 256)

    s = FADE( fx0 );

    n0 = grad1( perm[ ix0 ], fx0 );
    n1 = grad1( perm[ ix1 ], fx1 );
    return 0.188f * ( LERP( s, n0, n1 ) );
}


//---------------------------------------------------------------------
/** 2D float Perlin noise.
 */
static float noise2( float x, float y ) {
    int ix0, iy0, ix1, iy1;
    float fx0, fy0, fx1, fy1;
    float s, t, nx0, nx1, n0, n1;

    ix0 = FASTFLOOR( x ); // Integer part of x
    iy0 = FASTFLOOR( y ); // Integer part of y
    fx0 = x - ix0;        // Fractional part of x
    fy0 = y - iy0;        // Fractional part of y
    fx1 = fx0 - 1.0f;
    fy1 = fy0 - 1.0f;
    ix1 = (ix0 + 1) & 0xff;  // Wrap to 0..255
    iy1 = (iy0 + 1) & 0xff;
    ix0 = ix0 & 0xff;
    iy0 = iy0 & 0xff;

    t = FADE( fy0 );
    s = FADE( fx0 );

    nx0 = grad2(perm[ix0 + perm[iy0]], fx0, fy0);
    nx1 = grad2(perm[ix0 + perm[iy1]], fx0, fy1);
    n0 = LERP( t, nx0, nx1 );

    nx0 = grad2(perm[ix1 + perm[iy0]], fx1, fy0);
    nx1 = grad2(perm[ix1 + perm[iy1]], fx1, fy1);
    n1 = LERP(t, nx0, nx1);

    return 0.507f * ( LERP( s, n0, n1 ) );
}

//---------------------------------------------------------------------
/** 2D float Perlin periodic noise.
 */
static float pnoise2( float x, float y, int px, int py ) {
    int ix0, iy0, ix1, iy1;
    float fx0, fy0, fx1, fy1;
    float s, t, nx0, nx1, n0, n1;

    ix0 = FASTFLOOR( x ); // Integer part of x
    iy0 = FASTFLOOR( y ); // Integer part of y
    fx0 = x - ix0;        // Fractional part of x
    fy0 = y - iy0;        // Fractional part of y
    fx1 = fx0 - 1.0f;
    fy1 = fy0 - 1.0f;
    ix1 = (( ix0 + 1 ) % px) & 0xff;  // Wrap to 0..px-1 and wrap to 0..255
    iy1 = (( iy0 + 1 ) % py) & 0xff;  // Wrap to 0..py-1 and wrap to 0..255
    ix0 = ( ix0 % px ) & 0xff;
    iy0 = ( iy0 % py ) & 0xff;

    t = FADE( fy0 );
    s = FADE( fx0 );

    nx0 = grad2(perm[ix0 + perm[iy0]], fx0, fy0);
    nx1 = grad2(perm[ix0 + perm[iy1]], fx0, fy1);
    n0 = LERP( t, nx0, nx1 );

    nx0 = grad2(perm[ix1 + perm[iy0]], fx1, fy0);
    nx1 = grad2(perm[ix1 + perm[iy1]], fx1, fy1);
    n1 = LERP(t, nx0, nx1);

    return 0.507f * ( LERP( s, n0, n1 ) );
}

//---------------------------------------------------------------------
//////////////////////////////////////////////////// END OF PERLIN.C ////////////////////////////////////////////////////////

/////////////////////////////////////////////////// START OF SIMPLEMALLOC.C ////////////////////////////////////////////////
/*
 * Description: An implementation of the Perlin noise function by Ken Perlin in 1D and 2D
 * Version: 6/12/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
 */

char* leaf_charmalloc(int length) {
    return (char *) (malloc(length * sizeof(char)));
}

int* leaf_intmalloc(int length) {
    return (int *) (malloc(length * sizeof(int)));
}

float* leaf_floatmalloc(int length) {
    return (float *) (malloc(length * sizeof(float)));
}
////////////////////////////////////////////////// END OF SIMPLEMALLOC.C //////////////////////////////////////////////////////////

////////////////////////////////////////////////// START OF URLCREATOR.C /////////////////////////////////////////////////////////
/*
Description: A simple system in which you can create a .url file by setting the attributes of a shortcut using the struct Shortcut descripted in urlcreator.h
Version: 5/29/2021
Author(s): suncloudsmoon
Origin: N/A
License: MIT
*/

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
/////////////////////////////////////////////////// END OF URLCREATOR.C /////////////////////////////////////////////////////////

//////////////////////////////////////////////// START OF WJCRYPTLIB_SHA256.C ///////////////////////////////////////////////////

/*
 * Description: Hashes a string according to the SHA-2 specification
 * Origin: https://github.com/WaterJuice/WjCryptLib/blob/master/lib/WjCryptLib_Sha256.c
 * Version: [Origin: Mar 15, 2018], [Modified: May 10, 2021]
 * Author(s): Tom St Denis<tomstdenis@gmail.com>, WaterJuice<https://github.com/WaterJuice>, suncloudsmoon
 * License: Unlicense<https://github.com/WaterJuice/WjCryptLib/blob/master/UNLICENSE>
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  WjCryptLib_Sha256
//
//  Implementation of SHA256 hash function.
//  Original author: Tom St Denis, tomstdenis@gmail.com, http://libtom.org
//  Modified by WaterJuice retaining Public Domain license.
//
//  This is free and unencumbered software released into the public domain - June 2013 waterjuice.org
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  IMPORTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// #include <memory.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  MACROS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ror(value, bits) (((value) >> (bits)) | ((value) << (32 - (bits))))

#define MIN(x, y) ( ((x)<(y))?(x):(y) )

#define STORE32H(x, y)                                                                     \
     { (y)[0] = (uint8_t)(((x)>>24)&255); (y)[1] = (uint8_t)(((x)>>16)&255);   \
       (y)[2] = (uint8_t)(((x)>>8)&255); (y)[3] = (uint8_t)((x)&255); }

#define LOAD32H(x, y)                            \
     { x = ((uint32_t)((y)[0] & 255)<<24) | \
           ((uint32_t)((y)[1] & 255)<<16) | \
           ((uint32_t)((y)[2] & 255)<<8)  | \
           ((uint32_t)((y)[3] & 255)); }

#define STORE64H(x, y)                                                                     \
   { (y)[0] = (uint8_t)(((x)>>56)&255); (y)[1] = (uint8_t)(((x)>>48)&255);     \
     (y)[2] = (uint8_t)(((x)>>40)&255); (y)[3] = (uint8_t)(((x)>>32)&255);     \
     (y)[4] = (uint8_t)(((x)>>24)&255); (y)[5] = (uint8_t)(((x)>>16)&255);     \
     (y)[6] = (uint8_t)(((x)>>8)&255); (y)[7] = (uint8_t)((x)&255); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// The K array
static const uint32_t K[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
    0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
    0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
    0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
    0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
    0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
    0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
    0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
    0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
    0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
    0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

#define BLOCK_SIZE          64

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  INTERNAL FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Various logical functions
#define Ch( x, y, z )     (z ^ (x & (y ^ z)))
#define Maj( x, y, z )    (((x | y) & z) | (x & y))
#define S( x, n )         ror((x),(n))
#define R( x, n )         (((x)&0xFFFFFFFFUL)>>(n))
#define Sigma0( x )       (S(x, 2) ^ S(x, 13) ^ S(x, 22))
#define Sigma1( x )       (S(x, 6) ^ S(x, 11) ^ S(x, 25))
#define Gamma0( x )       (S(x, 7) ^ S(x, 18) ^ R(x, 3))
#define Gamma1( x )       (S(x, 17) ^ S(x, 19) ^ R(x, 10))

#define Sha256Round( a, b, c, d, e, f, g, h, i )       \
     t0 = h + Sigma1(e) + Ch(e, f, g) + K[i] + W[i];   \
     t1 = Sigma0(a) + Maj(a, b, c);                    \
     d += t0;                                          \
     h  = t0 + t1;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  TransformFunction
//
//  Compress 512-bits
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
void
    TransformFunction
    (
        Sha256Context*      Context,
        uint8_t const*      Buffer
    )
{
    uint32_t    S[8];
    uint32_t    W[64];
    uint32_t    t0;
    uint32_t    t1;
    uint32_t    t;
    int         i;

    // Copy state into S
    for( i=0; i<8; i++ )
    {
        S[i] = Context->state[i];
    }

    // Copy the state into 512-bits into W[0..15]
    for( i=0; i<16; i++ )
    {
        LOAD32H( W[i], Buffer + (4*i) );
    }

    // Fill W[16..63]
    for( i=16; i<64; i++ )
    {
        W[i] = Gamma1( W[i-2]) + W[i-7] + Gamma0( W[i-15] ) + W[i-16];
    }

    // Compress
    for( i=0; i<64; i++ )
    {
        Sha256Round( S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], i );
        t = S[7];
        S[7] = S[6];
        S[6] = S[5];
        S[5] = S[4];
        S[4] = S[3];
        S[3] = S[2];
        S[2] = S[1];
        S[1] = S[0];
        S[0] = t;
    }

    // Feedback
    for( i=0; i<8; i++ )
    {
        Context->state[i] = Context->state[i] + S[i];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sha256Initialise
//
//  Initialises a SHA256 Context. Use this to initialise/reset a context.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    Sha256Initialise
    (
        Sha256Context*      Context         // [out]
    )
{
    Context->curlen = 0;
    Context->length = 0;
    Context->state[0] = 0x6A09E667UL;
    Context->state[1] = 0xBB67AE85UL;
    Context->state[2] = 0x3C6EF372UL;
    Context->state[3] = 0xA54FF53AUL;
    Context->state[4] = 0x510E527FUL;
    Context->state[5] = 0x9B05688CUL;
    Context->state[6] = 0x1F83D9ABUL;
    Context->state[7] = 0x5BE0CD19UL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sha256Update
//
//  Adds data to the SHA256 context. This will process the data and update the internal state of the context. Keep on
//  calling this function until all the data has been added. Then call Sha256Finalise to calculate the hash.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    Sha256Update
    (
        Sha256Context*      Context,        // [in out]
        void const*         Buffer,         // [in]
        uint32_t            BufferSize      // [in]
    )
{
    uint32_t n;

    if( Context->curlen > sizeof(Context->buf) )
    {
       return;
    }

    while( BufferSize > 0 )
    {
        if( Context->curlen == 0 && BufferSize >= BLOCK_SIZE )
        {
           TransformFunction( Context, (uint8_t*)Buffer );
           Context->length += BLOCK_SIZE * 8;
           Buffer = (uint8_t*)Buffer + BLOCK_SIZE;
           BufferSize -= BLOCK_SIZE;
        }
        else
        {
           n = MIN( BufferSize, (BLOCK_SIZE - Context->curlen) );
           memcpy( Context->buf + Context->curlen, Buffer, (size_t)n );
           Context->curlen += n;
           Buffer = (uint8_t*)Buffer + n;
           BufferSize -= n;
           if( Context->curlen == BLOCK_SIZE )
           {
              TransformFunction( Context, Context->buf );
              Context->length += 8*BLOCK_SIZE;
              Context->curlen = 0;
           }
       }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sha256Finalise
//
//  Performs the final calculation of the hash and returns the digest (32 byte buffer containing 256bit hash). After
//  calling this, Sha256Initialised must be used to reuse the context.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    Sha256Finalise
    (
        Sha256Context*      Context,        // [in out]
        SHA256_HASH*        Digest          // [out]
    )
{
    int i;

    if( Context->curlen >= sizeof(Context->buf) )
    {
       return;
    }

    // Increase the length of the message
    Context->length += Context->curlen * 8;

    // Append the '1' bit
    Context->buf[Context->curlen++] = (uint8_t)0x80;

    // if the length is currently above 56 bytes we append zeros
    // then compress.  Then we can fall back to padding zeros and length
    // encoding like normal.
    if( Context->curlen > 56 )
    {
        while( Context->curlen < 64 )
        {
            Context->buf[Context->curlen++] = (uint8_t)0;
        }
        TransformFunction(Context, Context->buf);
        Context->curlen = 0;
    }

    // Pad up to 56 bytes of zeroes
    while( Context->curlen < 56 )
    {
        Context->buf[Context->curlen++] = (uint8_t)0;
    }

    // Store length
    STORE64H( Context->length, Context->buf+56 );
    TransformFunction( Context, Context->buf );

    // Copy output
    for( i=0; i<8; i++ )
    {
        STORE32H( Context->state[i], Digest->bytes+(4*i) );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sha256Calculate
//
//  Combines Sha256Initialise, Sha256Update, and Sha256Finalise into one function. Calculates the SHA256 hash of the
//  buffer.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    Sha256Calculate
    (
        void  const*        Buffer,         // [in]
        uint32_t            BufferSize,     // [in]
        SHA256_HASH*        Digest          // [in]
    )
{
    Sha256Context context;

    Sha256Initialise( &context );
    Sha256Update( &context, Buffer, BufferSize );
    Sha256Finalise( &context, Digest );
}

/////////////////////////////////////////////////////// END OF WJCRYPTLIB_SHA256.C //////////////////////////////////////
